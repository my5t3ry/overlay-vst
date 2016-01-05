#include "studio-link.h"
#include <pthread.h>
#include "re/re.h"
#include "baresip.h"

#define DEFAULT_PROG "Default"
#define UNIQUE_ID 'itsr'

#define EFFECT_NAME "StudioLink"
#define PRODUCT_STRING "StudioLink"
#define VENDOR_STRING "IT-Service Sebastian Reimers"
#define VENDOR_VERSION 1

#define MAX_GAIN 1

pthread_t tid;
bool running = false;


AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new vstplugin (audioMaster);
}

// Constructor
vstplugin::vstplugin(audioMasterCallback audiomaster)
: AudioEffectX(audioMaster, 1,3)
{
	setNumInputs(2);
	setNumOutputs(2);
	setUniqueID(UNIQUE_ID);
	canProcessReplacing();

	resume();

	vst_strncpy(programName,DEFAULT_PROG,kVstMaxProgNameLen);

	if (!running) {
		(void)sys_coredump_set(true);
		libre_init();
		conf_configure();
		ua_init("baresip v" BARESIP_VERSION " (" ARCH "/" OS ")",
				true, true, true, false);
		conf_modules();
		pthread_create(&tid, NULL, (void*(*)(void*))&re_main, NULL);
		running = true;
	}
	sess = effect_session_start();
	gain = 1;

}
// Destructor
vstplugin::~vstplugin()
{
	if (!effect_session_stop(sess)) {
		//re_cancel();
		ua_stop_all(false);
		//(void)pthread_join(tid, NULL);
		sys_msleep(1000);
		ua_close();
		mod_close();
		libre_close();
		//tmr_debug();
		//mem_debug();

		running = false;
	}

}

// Processing
void vstplugin::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float *input0 = inputs[0];
	float *output0 = outputs[0];
	float *input1 = inputs[1];
	float *output1 = outputs[1];

	effect_src(sess, input0, input1, sampleFrames);
	effect_play(sess, output0, output1, sampleFrames);
}

//Parameters
void vstplugin::setParameter(VstInt32 index, float value)
{
        gain = value*MAX_GAIN;
}

float vstplugin::getParameter(VstInt32 index)
{
        float param = 0.0;
        param = gain/MAX_GAIN;
        return param;
}

void vstplugin::getParameterName(VstInt32 index, char* label)
{
        vst_strncpy(label,"Gain",kVstMaxParamStrLen);
}

void vstplugin::getParameterDisplay(VstInt32 index, char* text)
{
        dB2string(gain, text, kVstMaxParamStrLen);
}

void vstplugin::getParameterLabel(VstInt32 index, char* label)
{
        vst_strncpy(label, "dB", kVstMaxParamStrLen);
}



// Plugin name set/get
void vstplugin::setProgramName(char* name)
{
	vst_strncpy(programName, name, kVstMaxProgNameLen);
}
void vstplugin::getProgramName(char* name)
{
	vst_strncpy(name, programName, kVstMaxProgNameLen);
}

//VST host functions
bool vstplugin::getEffectName(char* name) {
    vst_strncpy (name, EFFECT_NAME, kVstMaxEffectNameLen);
    return true;
}
bool vstplugin::getProductString(char* text) {
    vst_strncpy (text, PRODUCT_STRING, kVstMaxProductStrLen);
    return true;
}
bool vstplugin::getVendorString(char* text) {
    vst_strncpy (text, VENDOR_STRING, kVstMaxVendorStrLen);
    return true;
}
VstInt32 vstplugin::getVendorVersion() {
    return VENDOR_VERSION;
}

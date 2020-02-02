#ifndef DISTORTION_H
#define DISTORTION_H

#include "public.sdk/source/vst2.x/audioeffectx.h"

extern "C" {
	struct session;
	void effect_play(struct session *sess, float* const output0,
			float* const output1, unsigned long nframes);
	void effect_src(struct session *sess, const float* const input0,
			const float* const input1, unsigned long nframes);
	void effect_bypass(struct session *sess, float* const output0,
			float* const output1, const float* const input0,
			const float* const input1, unsigned long nframes);


	struct session* effect_session_start(void);
	int effect_session_stop(struct session *session);
}


class vstplugin:public AudioEffectX
{
	public:
		vstplugin(audioMasterCallback audioMaster);
		~vstplugin();
		void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames);
//		void resume();
    		// Programs for plug−in
    		void setProgramName (char* name);
    		void getProgramName (char* name);
    		// Parameters (use with GUI)

		void setParameter (VstInt32 index, float value);
		float getParameter (VstInt32 index);
		void getParameterLabel (VstInt32 index, char* label);
		void getParameterDisplay (VstInt32 index, char* text);
		void getParameterName (VstInt32 index, char* text);


		// Constructor
		// Destructor
    		// Plug−in details (name, etc.) for VST host
    		bool getEffectName (char* name);
    		bool getVendorString (char* text);
    		bool getProductString (char* text);
    		VstInt32 getVendorVersion ();
	private:
		enum{GAIN};
		char programName[kVstMaxProgNameLen + 1];
		float gain;
		struct session *sess;
};
#endif

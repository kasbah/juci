#include "../JuceLibraryCode/JuceHeader.h"

using namespace std;
class myCallback
    :public AudioIODeviceCallback
{
    public:
        myCallback() 
        {
            dry =  Atomic<float>();
        }
        ~myCallback() {;}
        void audioDeviceIOCallback (const float ** inputChannelData,
                                    int numInputChannels,
                                    float ** outputChannelData,
                                    int numOutputChannels,
                                    int numSamples );  

        void audioDeviceAboutToStart (AudioIODevice* device)
        {
            sampleRate = device->getCurrentSampleRate();
        }
        void audioDeviceStopped ()
        {
            sampleRate = 0;
        }

        Atomic<float> dry;

    private:
        double sampleRate;
};

void myCallback::audioDeviceIOCallback (const float ** inputChannelData,
                                        int numInputChannels,
                                        float ** outputChannelData,
                                        int numOutputChannels,
                                        int numSamples 
    )  
{
    if(numInputChannels == numOutputChannels)
    {
        for (int i = 0; i < numInputChannels; i++)
        {
            for (int j = 0; j < numSamples; j++)
            {
                outputChannelData[i][j] = inputChannelData[i][j] * dry.get();
            }
        }
    }

}

int main (int argc, char* argv[])
{
	ScopedJuceInitialiser_GUI plattform;
	AudioDeviceManager deviceManager;
    deviceManager.initialise(0,2,nullptr, false);
    
    //AudioDeviceSetup deviceSetup = getAudioDeviceSetup();
    cout << deviceManager.getCurrentAudioDeviceType() << endl; 
    deviceManager.setCurrentAudioDeviceType("JACK", true); 
    deviceManager.setMidiInputEnabled("Midi Through", true);
    cout << deviceManager.getCurrentAudioDeviceType() << endl; 

    myCallback* callback = new myCallback();
    //Thread::sleep(100);
    deviceManager.addAudioCallback(callback);

    for (float i = 0.0; i < 1.0; i+=0.01)
    {
        callback->dry.set(i);
        Thread::sleep(100);
    }

    return 0;
}

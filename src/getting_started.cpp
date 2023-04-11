/** \example getting_started.cpp
    This example describes the FIRST STEPS of handling Cam SDK.
    The given source code applies to handle one camera and image acquisition
    Copyright (c) by Baumer Optronic GmbH. All rights reserved, please see the provided license for full details.
*/

#include <iostream>
#include "neoapi/neoapi.hpp"
#include <thread>

// write the image callback
class TestNeoImageCallback : public NeoAPI::NeoImageCallback {
public:
    virtual void ImageCallback(const NeoAPI::Image& image) {
        std::cout << "Received image: " << image.GetImageID() <<
            " Timestamp: " << image.GetTimestamp() <<
            " Size: " << image.GetSize() << " Height: " << image.GetHeight() <<
            " Width: " << image.GetWidth() << " PixelFormat: " << image.GetPixelFormat() << std::endl;
    }
};

int main() {
    int result = 0;
    try {

        NeoAPI::Cam camera = NeoAPI::Cam();
        camera.Connect();
        // camera.f().ExposureTime.Set(10000);

        camera.f().TriggerMode = NeoAPI::TriggerMode::On;       // bring camera in TriggerMode
        camera.f().TriggerSource = NeoAPI::TriggerSource::Software;
        TestNeoImageCallback callback;
        camera.EnableImageCallback(callback);                   // enable the callback

        for (int i = 0; i < 5; i++) 
        {
            // send 5 triggers to retrieve some image callbacks
            camera.f().TriggerSoftware.Execute();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        while (1)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        camera.DisableImageCallback();                          // disable callback
    }
    catch (NeoAPI::NeoException& exc) {
        std::cout << "error: " << exc.GetDescription() << std::endl;
        result = 1;
    }
    catch (...) {
        std::cout << "oops, error" << std::endl;
        result = 1;
    }

    return result;
}

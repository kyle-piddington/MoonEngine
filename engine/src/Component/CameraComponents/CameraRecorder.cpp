#include <iostream>
#include <Geometry/Transform.h>
#include <MoonEngine.h>
#include "CameraRecorder.h"
#include <IO/TextLoader.h>
#include <thirdparty/rapidjson/prettywriter.h>
#include <fstream>

using namespace MoonEngine;

CameraRecorder::CameraRecorder():
    _recording(false),
    _counter(0),
    _savesPerSecond(3)
{

}

void CameraRecorder::start()
{

}

void CameraRecorder::writeJsonFile(rapidjson::Document & document)
{
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
    document.Accept(writer);

    std::ofstream file("cutscene.json");
    file << buffer.GetString();
}

rapidjson::Value CameraRecorder::serializeVec3(glm::vec3 obj, rapidjson::Document::AllocatorType& allocator) {
    rapidjson::Value array(rapidjson::kArrayType);
    array.PushBack(rapidjson::Value().SetFloat(obj.x), allocator);
    array.PushBack(rapidjson::Value().SetFloat(obj.y), allocator);
    array.PushBack(rapidjson::Value().SetFloat(obj.z), allocator);

    return array;
}

void CameraRecorder::saveSteps()
{
    std::string levelInfo = TextLoader::LoadFullFile(Library::getResourcePath() + "cutscene.json");

    rapidjson::Document document;
    document.Parse(levelInfo.c_str());

    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

    rapidjson::Value stepArray(rapidjson::kArrayType);

    for (auto step : _recordedSteps)
    {
        rapidjson::Value stepValue(rapidjson::kArrayType);

        stepValue.PushBack(serializeVec3(step.cameraPos, allocator), allocator);
        stepValue.PushBack(serializeVec3(step.lookAtPos, allocator), allocator);

        stepArray.PushBack(stepValue, allocator);
    }

    document.RemoveMember("steps");
    document.AddMember("steps", stepArray, allocator);

    std::cout << "Created step json file" << endl;

    writeJsonFile(document);
}

void CameraRecorder::update(float dt)
{
    ImGui::Begin("Camera Recorder");
    if (ImGui::Button("Record"))
    {
        _recording = !_recording;
    }
    if (ImGui::Button("Save"))
    {
        saveSteps();
    }


    if (_recording)
    {
        ImGui::Text("* Recording...");
        if (_counter >= 30 / _savesPerSecond)
        {
            Transform * transform = &gameObject->getTransform();
            _recordedSteps.push_back({transform->getPosition(), transform->forward()});
            LOG(INFO, "Saved a step");
            _counter = 0;
        }
        _counter++;
    }
    ImGui::End();
}

std::shared_ptr<Component> CameraRecorder::clone() const
{
    return std::make_shared<CameraRecorder>(*this);
}
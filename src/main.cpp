#ifndef URHO3D_CXX11
    #error ("C++11 doesn't supplied")
#endif

#include "application.hpp"



int RunApplication() {
    Urho3D::SharedPtr<Urho3D::Context> context {new Urho3D::Context {}};
    Urho3D::SharedPtr<Application> application {new Application {context}};
    return application->Run();
}

URHO3D_DEFINE_MAIN(RunApplication())

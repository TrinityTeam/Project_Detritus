#ifndef URHO3D_CXX11
    #error ("Undefined C++11")
#endif

#include "application.hpp"

/**
* This macro is expanded to (roughly, depending on OS) this:
*
* > int RunApplication()
* > {
* >     Urho3D::SharedPtr<Urho3D::Context> context(new Urho3D::Context());
* >     Urho3D::SharedPtr<className> application(new className(context));
* >     return application->Run();
* > }
* >
* > int main(int argc, char** argv)
* > {
* >     Urho3D::ParseArguments(argc, argv);
* >     return function;
* > }
*/
URHO3D_DEFINE_APPLICATION_MAIN(Application)

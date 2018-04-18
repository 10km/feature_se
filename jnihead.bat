ECHO OFF
pushd %~sdp0
javah -v -classpath ..\faceapi\faceapi-base\target\classes -d jni net.gdface.sdk.fse.FseJniBridge
popd
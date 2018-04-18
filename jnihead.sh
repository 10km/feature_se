src_path=`cd "$(dirname $0)";pwd`
javah -v -classpath $src_path/../faceapi-base/target/classes -d $src_path/jni net.gdface.sdk.fse.FseJniBridge


/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_mousebird_maply_VectorTileData */

#ifndef _Included_com_mousebird_maply_VectorTileData
#define _Included_com_mousebird_maply_VectorTileData
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_mousebird_maply_VectorTileData
 * Method:    getTileIDNative
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_com_mousebird_maply_VectorTileData_getTileIDNative
  (JNIEnv *, jobject);

/*
 * Class:     com_mousebird_maply_VectorTileData
 * Method:    getBoundsNative
 * Signature: (Lcom/mousebird/maply/Point2d;Lcom/mousebird/maply/Point2d;)V
 */
JNIEXPORT void JNICALL Java_com_mousebird_maply_VectorTileData_getBoundsNative
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     com_mousebird_maply_VectorTileData
 * Method:    getGeoBoundsNative
 * Signature: (Lcom/mousebird/maply/Point2d;Lcom/mousebird/maply/Point2d;)V
 */
JNIEXPORT void JNICALL Java_com_mousebird_maply_VectorTileData_getGeoBoundsNative
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     com_mousebird_maply_VectorTileData
 * Method:    getComponentObjects
 * Signature: ()[Lcom/mousebird/maply/ComponentObject;
 */
JNIEXPORT jobjectArray JNICALL Java_com_mousebird_maply_VectorTileData_getComponentObjects__
  (JNIEnv *, jobject);

/*
 * Class:     com_mousebird_maply_VectorTileData
 * Method:    getComponentObjects
 * Signature: (Ljava/lang/String;)[Lcom/mousebird/maply/ComponentObject;
 */
JNIEXPORT jobjectArray JNICALL Java_com_mousebird_maply_VectorTileData_getComponentObjects__Ljava_lang_String_2
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_mousebird_maply_VectorTileData
 * Method:    addComponentObject
 * Signature: (Lcom/mousebird/maply/ComponentObject;)V
 */
JNIEXPORT void JNICALL Java_com_mousebird_maply_VectorTileData_addComponentObject
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_mousebird_maply_VectorTileData
 * Method:    addComponentObjects
 * Signature: ([Lcom/mousebird/maply/ComponentObject;)V
 */
JNIEXPORT void JNICALL Java_com_mousebird_maply_VectorTileData_addComponentObjects
  (JNIEnv *, jobject, jobjectArray);

/*
 * Class:     com_mousebird_maply_VectorTileData
 * Method:    getVectors
 * Signature: ()[Lcom/mousebird/maply/VectorObject;
 */
JNIEXPORT jobjectArray JNICALL Java_com_mousebird_maply_VectorTileData_getVectors
  (JNIEnv *, jobject);

/*
 * Class:     com_mousebird_maply_VectorTileData
 * Method:    nativeInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_mousebird_maply_VectorTileData_nativeInit
  (JNIEnv *, jclass);

/*
 * Class:     com_mousebird_maply_VectorTileData
 * Method:    initialise
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_mousebird_maply_VectorTileData_initialise__
  (JNIEnv *, jobject);

/*
 * Class:     com_mousebird_maply_VectorTileData
 * Method:    initialise
 * Signature: (IIILcom/mousebird/maply/Point2d;Lcom/mousebird/maply/Point2d;Lcom/mousebird/maply/Point2d;Lcom/mousebird/maply/Point2d;)V
 */
JNIEXPORT void JNICALL Java_com_mousebird_maply_VectorTileData_initialise__IIILcom_mousebird_maply_Point2d_2Lcom_mousebird_maply_Point2d_2Lcom_mousebird_maply_Point2d_2Lcom_mousebird_maply_Point2d_2
  (JNIEnv *, jobject, jint, jint, jint, jobject, jobject, jobject, jobject);

/*
 * Class:     com_mousebird_maply_VectorTileData
 * Method:    dispose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_mousebird_maply_VectorTileData_dispose
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif

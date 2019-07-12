//
// Created by Yongpan Song on 2019/4/1.
//

#include "com_duoyi_dyhttp_DuoyiHttpApi.h"
#include <android/log.h>
#include <curl/curl.h>
#include <utils/dyutils.h>
#include <safety/md5/md5.h>
#include <string.h>
#include <errno.h>
#include <sstream>
#define SKIP_PEER_VERIFICATION
#define SKIP_HOSTNAME_VERIFICATION

static const char *ALPHA_BASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    LOGI("write_data:%s", (char*)ptr);
    return size * nmemb;
}

static size_t Handle_callback(char *buffer, size_t size, size_t nitems, void *userData) {
    LOGI("Handle_callback %s", buffer);
    return nitems * size;
}

static void enBASE64code( const char *charBuf,    int charBufLen,    char *base64Char, long int &base64CharLen)
{
    int a = 0;
    int i = 0;
    while (i < charBufLen)
    {
        char b0 = charBuf[i++];
        char b1 = (i < charBufLen) ? charBuf[i++] : 0;
        char b2 = (i < charBufLen) ? charBuf[i++] : 0;

        int int63 = 0x3F;   //  00111111 意图去掉8bit的两高位
        int int255 = 0xFF;  // 11111111
        base64Char[a++] = ALPHA_BASE[(b0 >> 2) & int63];    //b0字符的前6bit   00+6bit 组成一个十进制数 即BASE编码表号
        base64Char[a++] = ALPHA_BASE[((b0 << 4) | ((b1 & int255) >> 4)) & int63];   //b0 要想取最后两位，所以左移四位 得到最后两位 再和b1字符的 前四个字符做  或运算 组成6bit  00+6bit
        base64Char[a++] = ALPHA_BASE[((b1 << 2) | ((b2 & int255) >> 6)) & int63];   //b1 要想得到后四位  需要右移2位 再和 b2的前4 位做或运算 组成6bit   00+bit 租组成BASE64编码
        base64Char[a++] = ALPHA_BASE[b2 & int63];  //b2的后6位   00+6bit 组成BASE编码号
    }
    base64Char[a] = '\0';
    //最后不够4个字符  补=，最多补两个 ==
    switch (charBufLen % 3)
    {
        case 1:
            base64Char[--a] = '=';
        case 2:
            base64Char[--a] = '=';
    }
    base64CharLen=strlen(base64Char); //输出的base64长度

}

static void deBASE64code(const char *base64Char,  int base64CharLen, char *outStr,  long int &outStrLen)
{
    int toInt[128] = {-1};
    for (int i = 0; i < 64; i++) //将ANSI 码找到对应的BASE64码
    {
        int pos=ALPHA_BASE[i];
        toInt[pos] = i;
    }
    int int255 = 0xFF;  //11111111
    int index = 0;
    outStrLen = 0;
    for (int i = 0; i < base64CharLen; i += 4)
    {
        int c0 = toInt[base64Char[i]]; // 获取一组中第一个BASE64编码号
        int c1 = toInt[base64Char[i + 1]];// 获取一组中第二个BASE64编码号
        outStr[index++] = (((c0 << 2) | (c1 >> 4)) & int255);  //c0后6位  和c1去完前2位的  的前2位  组成8bit 再转换成字符
        outStrLen++;
        if (index >= base64CharLen)  //如果长度小于输入进来的长度 意图去掉=
        {
            return ;
        }
        int c2 = toInt[base64Char[i + 2]];// 获取一组中第三个BASE64编码号
        outStr[index++] = (((c1 << 4) | (c2 >> 2)) & int255);  //c1的后6位和c2去完前2位的  的前2位 组成8bit  转换成字符
        outStrLen++;
        if (index >= outStrLen)  //如果长度小于输入进来的长度  意图去掉=
        {
            return ;
        }
        int c3 = toInt[base64Char[i + 3]];// 获取一组中第四个BASE64编码号
        outStr[index++] = (((c2 << 6) | c3) & int255); //c2的后2位和c3的后6位  组成8bit 转换成字符
        outStrLen++;
    }
    //outStrLen=strlen(outStr);
}

JNIEXPORT void JNICALL Java_com_duoyi_dyhttp_DuoyiHttpApi_DyHttpInit(JNIEnv *env, jobject, jstring jPath) {
    LOGI("start upload");
    char rpath[128] = { 0 };
    jboolean is_copy = JNI_FALSE;
    const char *java_path = env->GetStringUTFChars(jPath, &is_copy);
    strcpy(rpath, java_path);
    env->ReleaseStringUTFChars(jPath, java_path);
    LOGI("java_path:%s", rpath);
    char name[56] = "crash.txt";
    long int amr_len = 0;
    long int decode_len = 0;
    long int base64_len = 0;
    char amr_data [10240];
    char json_data[10240];
    char decode_data [10240];
    FILE *dump_file = fopen(rpath, "rb");
    if (dump_file == NULL) {
        LOGE("file not exist %s", strerror(errno));
    } else {
        fseek(dump_file, 0, SEEK_END);
        amr_len = ftell(dump_file);
        fseek(dump_file, 0, SEEK_SET);
        char* temp_data = (char*)malloc(sizeof(char) * amr_len);
        fread(temp_data, sizeof(char), amr_len, dump_file);

        fclose(dump_file);
        LOGE("file  exist %ld", amr_len);
        dump_file = NULL;
        LOGE("amr_data %s", temp_data);
        enBASE64code(temp_data, amr_len, json_data, base64_len);
    }

    deBASE64code(json_data, base64_len,decode_data,decode_len);
    //FILE *dump_file1 = fopen("/storage/emulated/0/MediaBitFile1.amr", "wb");
    //fwrite(decode_data, sizeof(char), decode_len, dump_file1);
    //fclose(dump_file1);
    //dump_file1 = NULL;
    LOGE("file  exist %ld", decode_len);
    LOGE("file  exist %ld", base64_len);
    LOGE("file  exist %ld,%ld", strlen(amr_data),strlen(json_data));
    string json_data2;
    json_data2 = json_data;

    char result[128] = {0};
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (curl) {
        std::string strJson = "{";
        strJson += "\"sysId\" : \"m2sw\",";
        strJson += "\"taskId\" : \"fjajfsdkjfs\",";
        strJson += "\"sendTime\" : \"498273498\",";
        strJson += "\"audioUrl\" : \"http://et.oa.com:6060/gv/pp/190515/1521/852282f14b828d.mdya\",";
        strJson += "\"audio_rate\" : \"8k\",";
        strJson += "\"audio_format\" : \"amr\",";
        strJson += "\"audio\" : \"";
        LOGE("strJson %s", strJson.c_str());
        std::string testjson = strJson + json_data2;
        LOGE("testjson %s", testjson.c_str());
        LOGE("file  exist %ld", testjson.size());
        std::string strJson1 = "\",";
        strJson1 += "\"cbExt\" : \"dk\",";
        strJson1 += "\"scene\" : \"all\"";
        strJson1 += "}";

        testjson += strJson1;
        LOGE("testjson %s", testjson.c_str());
        struct curl_httppost *formpost = NULL;
        struct curl_httppost *lastptr = NULL;

//        curl_formadd(&formpost, &lastptr,
//                CURLFORM_COPYNAME, "product",
//                CURLFORM_COPYCONTENTS, "dymediasdk",
//                CURLFORM_END);
//
//        curl_formadd(&formpost, &lastptr,
//                CURLFORM_COPYNAME, "uid",
//                CURLFORM_COPYCONTENTS, "123456",
//                CURLFORM_END);
//
//        curl_formadd(&formpost, &lastptr,
//                CURLFORM_COPYNAME, "files",
//                CURLFORM_FILE, rpath,
//                CURLFORM_CONTENTTYPE, "application/octet-stream",
//                CURLFORM_END);
//
//        LOGI("java_path:%s", rpath);
//
//        char key_info[128] = { 0 };
//        char token_info[] = "KTtD9Jx7XMES10V5";
//        sprintf(key_info, "%s%s", name, token_info);
//
//        const unsigned char *digest;
//        MD5 md5_util(key_info);
//        digest = md5_util.getDigest();
//        curl_formadd(&formpost, &lastptr,
//                CURLFORM_COPYNAME, "key",
//                CURLFORM_COPYCONTENTS, md5_util.toStr().c_str(),
//                CURLFORM_END);
//        LOGD("md5 %s", digest);
//
//        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, Handle_callback);
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
//
//        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
//        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
//
//        curl_easy_setopt(curl, CURLOPT_URL, "https://zmlog.duoyi.com:38433/api/voice/logfileupload");
        std::stringstream out;
        LOGI("10.32.72.42:8096/asr_api ");
        curl_easy_setopt(curl, CURLOPT_URL, "10.32.72.42:8096/asr_api ");
        //构建HTTP报文头
        struct curl_slist* headers = NULL;
        headers=curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        //curl_easy_setopt(curl,  CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_POST, 1);//设置为非0表示本次操作为POST
        // 设置要POST的JSON数据

        //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, testJson.c_str());//以多字节上传
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, testjson.c_str());//以Unicode编码上传
        //curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strJson.size());
        // 设置接收数据的处理函数和存放变量
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, Handle_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);//设置回调函数
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);//设置写数据

        res = curl_easy_perform(curl);

        curl_slist_free_all(headers); /* free the list again */
//#ifdef SKIP_PEER_VERIFICATION
//        /*
//         * If you want to connect to a site who isn't using a certificate that is
//         * signed by one of the certs in the CA bundle you have, you can skip the
//         * verification of the server's certificate. This makes the connection
//         * A LOT LESS SECURE.
//         *
//         * If you have a CA cert for the server stored someplace else than in the
//         * default bundle, then the CURLOPT_CAPATH option might come handy for
//         * you.
//         */
//        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
//#endif
//
//#ifdef SKIP_HOSTNAME_VERIFICATION
//        /*
//         * If the site you're connecting to uses a different host name that what
//         * they have mentioned in their server certificate's commonName (or
//         * subjectAltName) fields, libcurl will refuse to connect. You can skip
//         * this check, but this will make the connection less secure.
//         */
//        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
//#endif
//
//        /* Perform the request, res will get the return code */
//        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK) {
            curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, result);
            __android_log_print(ANDROID_LOG_ERROR, "ALLTAG", "curl_easy_perform() fail:%s,url:%s",
                                curl_easy_strerror(res), result);
        } else {
            __android_log_print(ANDROID_LOG_INFO, "ALLTAG", "curl_easy_perform() success:%s",
                                curl_easy_strerror(res));
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    } else {
        __android_log_print(ANDROID_LOG_ERROR, "ALLTAG", "curl_easy_perform() curl NULL");
    }

    curl_global_cleanup();
}
//
// Created by Administrator on 2019/10/16 0016.
//

#ifndef FFMPEGDEMO_DEMUXER_TEST_H
#define FFMPEGDEMO_DEMUXER_TEST_H
#include <stdio.h>

/*FFmpeg����C���Ա�д�ģ������������������C++��д��������FFmpeg��ͷ�ļ�ʱ�������extern "C"���������ᱨ��"�޷��������ⲿ����......"*/
extern "C" {
#include "libavformat/avformat.h"
}

/**
* @brief ��һ��AVRational���͵ķ���ת��Ϊdouble���͵ĸ�����
* @param r:rΪһ��AVRational���͵Ľṹ���������Աnum��ʾ���ӣ���Աden��ʾ��ĸ��r��ֵ��Ϊ(double)r.num / (double)r.den�������ַ�����ʾ�������̶ȵر��⾫�ȵ���ʧ
* @return �������r�ķ�ĸdenΪ0���򷵻�0��Ϊ�˱������Ϊ0���³���������;�����������(double)r.num / (double)r.den
*/
static double r2d(AVRational r)
{
    return r.den == 0 ? 0 : (double)r.num / (double)r.den;
}


int main_demuxe_test()
{
    //const char *path = "E:\\QtProjects\\C++_ffmpeg_Qt_player\\Projects\\Test21_1A\\video1.mp4"; //��Ҫ��ȡ�ı���ý���ļ�����·����E:\QtProjects\C++_ffmpeg_Qt_player\Projects\Test21_1A\video1.mp4�£��������ڳ�����windowsƽ̨�������ַ����е�·�����á�\\���ָ�
    const char *path = "../resources/����.mp4"; //��Ҫ��ȡ�ı���ý���ļ����·��Ϊvideo1.mp4�����������ļ�video1.mp4���ڹ���Ŀ¼�£��������·��Ϊvideo1.mp4
    //const char *path = "audio1.mp3";
    av_register_all(); //��ʼ�����������ֻ�е����˸ú���������ʹ�ø������ͱ�����������򣬵��ú���avformat_open_input��ʧ�ܣ��޷���ȡý���ļ�����Ϣ
    avformat_network_init(); //�����������������ֻ��Ҫ��ȡ����ý���ļ�������Ҫ�õ����繦�ܣ����Բ��ü�����һ��
    AVDictionary *opts = NULL;
    AVFormatContext *ic = NULL;    //AVFormatContext������һ��ý���ļ���ý�����Ĺ��ɺͻ�����Ϣ�Ľṹ��
    int re = avformat_open_input(&ic, path, NULL, &opts); //ý��򿪺��������øú������Ի��·��Ϊpath��ý���ļ�����Ϣ��������Щ��Ϣ���浽ָ��icָ��Ŀռ��У����øú���������һ���ռ䣬��ָ��icָ��ÿռ䣩
    if (re != 0)  //�����ý���ļ�ʧ�ܣ���ӡʧ��ԭ�򡣱��磬�������û�е��ú���av_register_all������ӡ��XXX failed!:Invaliddata found when processing input��
    {
        char buf[1024] = { 0 };
        av_strerror(re, buf, sizeof(buf) - 1);
        printf("open %s failed!:%s", path, buf);
    }
    else         //��ý���ļ��ɹ�
    {
        printf("��ý���ļ� %s �ɹ�!\n", path);
        avformat_find_stream_info(ic, NULL); //���øú������Խ�һ����ȡһ��������Ƶ���ݲ��һ��һЩ��ص���Ϣ������avformat_open_input֮�������޷���ȡ����ȷ�����е�ý����������Ի���Ҫ����avformat_find_stream_info��һ����ȥ��ȡ��
        printf("ý���ļ�����:%s\n", ic->filename); //����avformat_open_input��ȡ����ý���ļ���·��/����
        printf("����Ƶ���ĸ���:%d\n", ic->nb_streams);//����Ƶ���ĸ��������һ��ý���ļ�������Ƶ��������Ƶ����nb_streams��ֵΪ2�����ý���ļ�ֻ����Ƶ����ֵΪ1
        //printf("duration:%d\n", ic->duration);
        printf("ý���ļ���ƽ������:%lldbps\n", ic->bit_rate);    //ý���ļ���ƽ������,��λΪbps
        int tns, thh, tmm, tss;
        tns = (ic->duration) / AV_TIME_BASE;
        thh = tns / 3600;
        tmm = (tns % 3600) / 60;
        tss = (tns % 60);
        printf("ý���ļ���ʱ����%dʱ%d��%d��\n", thh, tmm, tss); //ͨ���������㣬���Եõ�ý���ļ�����ʱ��

        printf("\n");
        for (int i = 0; i < ic->nb_streams; i++) //ͨ�������ķ�ʽ��ȡý���ļ���Ƶ����Ƶ����Ϣ���°汾��FFmpeg�������˺���av_find_best_stream��Ҳ����ȡ��ͬ����Ч����������Ϊ�˼��ݾɰ汾���������ֱ����ķ�ʽ
        {
            AVStream *as = ic->streams[i];
            if (AVMEDIA_TYPE_AUDIO == as->codecpar->codec_type) //�������Ƶ�������ӡ��Ƶ����Ϣ
            {
                printf("��Ƶ��Ϣ:\n");
                printf("index:%d\n", as->index);  //���һ��ý���ļ�������Ƶ��������Ƶ������Ƶindex��ֵһ��Ϊ1������ֵ��һ��׼ȷ�����Ի��ǵ�ͨ��as->codecpar->codec_type�ж�����Ƶ������Ƶ
                printf("��Ƶ������:%dHz\n", as->codecpar->sample_rate); //��Ƶ��������Ĳ����ʣ���λΪHz
                if (AV_SAMPLE_FMT_FLTP == as->codecpar->format)   //��Ƶ������ʽ
                {
                    printf("��Ƶ������ʽ:AV_SAMPLE_FMT_FLTP\n");
                }
                else if (AV_SAMPLE_FMT_S16P == as->codecpar->format)
                {
                    printf("��Ƶ������ʽ:AV_SAMPLE_FMT_S16P\n");
                }
                printf("��Ƶ�ŵ���Ŀ:%d\n", as->codecpar->channels); //��Ƶ�ŵ���Ŀ
                if (AV_CODEC_ID_AAC == as->codecpar->codec_id)   //��Ƶѹ�������ʽ
                {
                    printf("��Ƶѹ�������ʽ:AAC\n");
                }
                else if (AV_CODEC_ID_MP3 == as->codecpar->codec_id)
                {
                    printf("��Ƶѹ�������ʽ:MP3\n");
                }
                int DurationAudio = (as->duration) * r2d(as->time_base); //��Ƶ��ʱ������λΪ�롣ע������ѵ�λ�Ŵ�Ϊ�������΢���Ƶ��ʱ������Ƶ��ʱ����һ����ȵ�
                printf("��Ƶ��ʱ����%dʱ%d��%d��\n", DurationAudio / 3600, (DurationAudio % 3600) / 60, (DurationAudio % 60)); //����Ƶ��ʱ��ת��Ϊʱ����ĸ�ʽ��ӡ������̨��
                printf("\n");
            }
            else if (AVMEDIA_TYPE_VIDEO == as->codecpar->codec_type)  //�������Ƶ�������ӡ��Ƶ����Ϣ
            {
                printf("��Ƶ��Ϣ:\n");
                printf("index:%d\n", as->index);   //���һ��ý���ļ�������Ƶ��������Ƶ������Ƶindex��ֵһ��Ϊ0������ֵ��һ��׼ȷ�����Ի��ǵ�ͨ��as->codecpar->codec_type�ж�����Ƶ������Ƶ
                printf("��Ƶ֡��:%lffps\n", r2d(as->avg_frame_rate)); //��Ƶ֡��,��λΪfps����ʾÿ����ֶ���֡
                switch (as->codecpar->codec_id){
                    case AV_CODEC_ID_H264:
                        cout<<"��Ƶѹ�������ʽ��h264"<<endl;

                        break;
                    case AV_CODEC_ID_MPEG4:
                        cout<<"��Ƶѹ�������ʽ��mpeg4"<<endl;

                        break;
                    case AV_CODEC_ID_H265:
                        cout<<"��Ƶѹ�������ʽ��h265"<<endl;
                        break;
                }

                cout<<"��Ƶѹ�������ʽ��"<<(as->codecpar->codec_id)<<endl;
                printf("֡���:%d ֡�߶�:%d\n", as->codecpar->width, as->codecpar->height); //��Ƶ֡��Ⱥ�֡�߶�
                int DurationVideo = (as->duration) * r2d(as->time_base); //��Ƶ��ʱ������λΪ�롣ע������ѵ�λ�Ŵ�Ϊ�������΢���Ƶ��ʱ������Ƶ��ʱ����һ����ȵ�
                printf("��Ƶ��ʱ����%dʱ%d��%d��\n", DurationVideo / 3600, (DurationVideo % 3600) / 60, (DurationVideo % 60)); //����Ƶ��ʱ��ת��Ϊʱ����ĸ�ʽ��ӡ������̨��
                printf("\n");
            }
        }

        //av_dump_format(ic, 0, path, 0);
    }
    if (ic)
    {
        avformat_close_input(&ic); //�ر�һ��AVFormatContext,�ͺ���avformat_open_input()�ɶ�ʹ��
    }
    getchar(); //������һ�䣬��ֹ�����ӡ����Ϣ�������˳���
    return 0;
}
#endif //FFMPEGDEMO_DEMUXER_TEST_H

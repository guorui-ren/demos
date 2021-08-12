/**
**功能描述：测试获取时间接口的性能，
	接口包括：clock_gettime gettimeofday time ftime gmttime gmttime_r localtime localtime_r timegm 
**创建者: guorui-ren@foxmail.com
**创建时间:2021.08.22
**备注：gcc -g -O2 -lpthread -lrt main.c -o demo_time
**/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>
#include <sys/timeb.h>

int g_thread_num = 0;
int g_thread_run_count = 0;

typedef void*(thread_func)(void*);


enum TIME_INTERFACE
{
	TIME_TIME,
	TIME_GETTIMEOFDAY,
	TIME_CLOCK_GETTIME,
	TIME_FTIME,
	TIME_LOCALTIME,
	TIME_LOCALTIME_R,
	TIME_GMTIME,
	TIME_GMTIME_R,
	TIME_TIMEGM,
	TIME_DIFF,
	TIME_NUM
};

static void* thread_run_func(void* arg)
{
	int time_interface = *(int*)arg;
	
	int i = 0;
	for(; i < g_thread_run_count; i++)
	{
		switch(time_interface)
		{
			case TIME_TIME:
			{
				time(NULL);
				break;
			}
			case TIME_GETTIMEOFDAY:
			{
				struct timeval tv;
				gettimeofday(&tv, NULL);
				break;
			}
			case TIME_CLOCK_GETTIME:
			{
				struct timespec ts;
				clock_gettime(CLOCK_REALTIME_COARSE, &ts);
				break;
			}
			case TIME_FTIME:
			{
				struct timeb tb;
				ftime(&tb);
				break;
			}
			case TIME_LOCALTIME:
			{
				time_t time_local;
				struct tm * tm_local = localtime(&time_local);
				break;
			}
			case TIME_LOCALTIME_R:
			{
				time_t time_local;
				struct tm tm_local; 
				localtime_r(&time_local, &tm_local);
				break;
			}
			case TIME_GMTIME:
			{
				time_t time_gmt;
				struct tm * tm_gmt = gmtime(&time_gmt);
				break;
			}
			case TIME_GMTIME_R:
			{
				time_t time_gmt;
				struct tm tm_gmt; 
				gmtime_r(&time_gmt, &tm_gmt);
				break;
			}
			case TIME_TIMEGM:
			{
				struct tm tm_gmt;
				timegm(&tm_gmt);
				break;
			}
			case TIME_DIFF:
			{
				struct timespec ts;
				clock_gettime(CLOCK_REALTIME_COARSE, &ts);
				time_t clock_sec = ts.tv_sec;
				long clock_nsec = ts.tv_nsec;
				long clock_usec = ts.tv_nsec/1000;

				struct timeval tv;
				gettimeofday(&tv, NULL);
				time_t time_sec = tv.tv_sec;
				long time_usec = tv.tv_usec;

				if(clock_sec != time_sec || clock_usec != time_usec)
				{
					printf("dff, time=[%d, %d], clock=[%d, %d]\n", time_sec, time_usec, clock_sec, clock_usec);
				}
				break;
			}
			default:
			{
				assert(0);
				break;
			}
		}
	}
	return NULL;
}

const char* get_func_name(enum TIME_INTERFACE time_interface)
{
	static char * time_i = NULL;
	time_i = "unknown";
	
	switch(time_interface)
	{
		case TIME_TIME:
		{
			time_i = "time";
			break;
		}
		case TIME_GETTIMEOFDAY:
		{
			time_i = "gettimeofday";
			break;
		}
		case TIME_CLOCK_GETTIME:
		{
			time_i = "clock_gettime";
			break;
		}
		case TIME_FTIME:
		{
			time_i = "ftime";
			break;
		}
		case TIME_LOCALTIME:
		{
			time_i = "localtime";
			break;
		}
		case TIME_LOCALTIME_R:
		{
			time_i = "localtime_r";
			break;
		}
		case TIME_GMTIME:
		{
			time_i = "gmtime";
			break;
		}
		case TIME_GMTIME_R:
		{
			time_i = "gmtime_r";
			break;
		}
		case TIME_TIMEGM:
		{
			time_i = "timegm";
			break;
		}
		case TIME_DIFF:
		{
			time_i = "time_diff";
			break;
		}
		default:
		{
			assert(0);
			break;
		}
	}
	
	return time_i;
}

void thread_run_pool(enum TIME_INTERFACE time_interface)
{
	pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * g_thread_num);

	struct timeval begin, end;

	gettimeofday(&begin, NULL);

	int i = 0;
	for (i = 0; i < g_thread_num; i++)
	{
		pthread_create(&threads[i], NULL, thread_run_func, &time_interface);
	}

	for (i = 0; i < g_thread_num; i++)
	{
		pthread_join(threads[i], NULL);
	}

	gettimeofday(&end, NULL);
	
	printf("%s delay = %lu ms, %d s, %d us\n", get_func_name(time_interface),
	((end.tv_sec - begin.tv_sec) * 1000000 + (end.tv_usec - begin.tv_usec))/1000, 
	end.tv_sec - begin.tv_sec, end.tv_usec - begin.tv_usec);
}

int main(int argc, const char* argv[])
{
	if(argc != 3)
	{
		printf("param error!, usage, demo_time thread_num thread_run_count\n");
		return -1;
	}
	g_thread_num = atoi(argv[1]);

	g_thread_run_count = atoi(argv[2]);

	printf("thread:%d, count:%d\n", g_thread_num, g_thread_run_count);

	thread_run_pool(TIME_CLOCK_GETTIME);

	thread_run_pool(TIME_GETTIMEOFDAY);

	thread_run_pool(TIME_TIME);
	
	thread_run_pool(TIME_FTIME);
	
	thread_run_pool(TIME_GMTIME);
	
	thread_run_pool(TIME_GMTIME_R);
	
	thread_run_pool(TIME_LOCALTIME);
	
	thread_run_pool(TIME_LOCALTIME_R);
	
	thread_run_pool(TIME_TIMEGM);

	//thread_run_pool(TIME_DIFF);

	//printf("CLOCK_REALTIME=%d\n", CLOCK_REALTIME);
	//printf("CLOCK_REALTIME_COARSE=%d\n", CLOCK_REALTIME_COARSE);
	
    return 0;
}




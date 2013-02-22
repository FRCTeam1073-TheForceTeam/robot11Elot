#include "Retros.h"
#include "Math.h"

#ifndef RETROS_H_
#define RETROS_H_

#define MAX 255

#define PIXELSIZE 3 // # of uchars in a pixel
#define RIND 0
#define GIND 1
#define BIND 2

typedef unsigned char uchar;
typedef unsigned int uint;

#if 0
float* RetroDetector::Discrepancy(
			uchar*R0, uchar*G0, uchar*B0, 
			uchar*R1, uchar*G1, uchar*B1,
			Change R, Change G, Change B,
			int channelsused,
			float &max,float &min
			)
{
	int size = sizeof(R0)/sizeof(uchar);
	
	float discreps[size]; // the array of float values representing discrepancies
	max = -99999; // keep track of largest and smallest values
	min = 99999;
	for(int i = 0; i < size; i++)
	{
		float dR = discrep(R0[i],R1[i],R,3);
		float dG = discrep(G0[i],G1[i],G,3);
		float dB = discrep(B0[i],B1[i],B,3);
		float res = pow(dR*dG*dB,1.0/channelsused);
		discreps[i] = res;
		if(res > max){max = res;}
		if(res < min){min = res;}
	}
	return discreps;
}
#else
uint* RetroDetector::Discrepancy(
			uchar*i0, 
			uchar*i1,
			Change R, Change G, Change B,
			uint &max,uint &min
			)
{
	int size = sizeof(i0)/(sizeof(uchar) * PIXELSIZE);
	
	uint discreps[size]; // the array of float values representing discrepancies
	max = 0; // keep track of largest and smallest values
	min = 99999;
	for(int i = 0,ind=0; i < size; i++,ind += PIXELSIZE)
	{
		uint dR = discrep(i0[ind+RIND],i1[ind+RIND],R);
		uint dG = discrep(i0[ind+GIND],i1[ind+GIND],G);
		uint dB = discrep(i0[ind+BIND],i1[ind+BIND],B);
		
		uint res = dR*dG*dB+1;
		discreps[i] = res;
		if(res > max){max = res;}
		if(res < min){min = res;}
	}
	return NULL; // discreps;  Remove "warning: address of local variable `discreps' returned"
}
#endif

#define chR LL
#define chG LH
#define chB Undef
#define THRESHLEVEL .6
#define MINXOFF 5
#define MINYOFF 5
#define MAXXOFF 25
#define MAXYOFF 25
#define MINPIXNUM 14
Point* RetroDetector::Tapes(
	uchar * i0, uchar * i1,
	int W, int H,
	int &tapes
	)
{
	uint max;
	uint min;
	uint* discreps = Discrepancy(i0,i1,chR,chG,chB,max,min);
	
	uint thresh = (uint)(THRESHLEVEL * (max - min) + min);
	
	tapes = 0;
	Point res[99];
		
	int i = 0;
	for(int x = 0; x < W; x++)
	{
		for(int y = 0; y < H; y++,i++)
		{
			if(discreps[i]>thresh)
			{
				int xmin = x - MINXOFF;
				if(xmin < 0){xmin = 0;} // don't go off edge of image
				int ymin = y - MINYOFF;
				if(ymin < 0){ymin = 0;} // don't go off edge of image
				int xmax = x + MAXXOFF;
				if(xmax > W){xmax = W;} // don't go off edge of image
				int ymax = y + MAXYOFF;
				if(ymax > H){ymax = H;} // don't go off edge of image
				
				int num = 0;
				int xtot = 0;
				int ytot = 0;
				for(int _y = ymin; _y < ymax; _y++)
				{
					int ind = _y * W + xmin;
					for(int _x = xmin; _x < xmax; _x++,ind++)
					{
						if(discreps[ind] > thresh)
						{
							num++;
							xtot+=_x;
							ytot+=_y;
						}
					}
				}
				if(num >= MINPIXNUM)
				{
					res[tapes].x=((float)(xtot))/num;
					res[tapes].y=((float)(ytot))/num;
					tapes++;
				}
			}
		}
	}
	return NULL; // res;  Remove "warning: address of local variable `res' returned"
}

#define KK 3
uint RetroDetector::discrep(uchar _0,uchar _1,Change c)
{
#if 0
	float _a = MAX - _0;
	float _b = MAX - _1;
#else
	uint _a = (MAX-(uint)_0)*(MAX-(uint)_0);
	uint _b = (MAX-(uint)_1)*(MAX-(uint)_1);
#endif
	switch (c)
	{
		case HH: return (65536 * KK) / (_a*_b+KK); break;
	    case HL: return ((_b + KK)*KK) / (_a + KK); break;
	    case LH: return ((_a + KK)*KK) / (_b + KK); break;
	    case LL: return (_a/MAX) * (_b/MAX); break;
	    case Undef: return 1; break;
	}
	return 0;
}

#endif

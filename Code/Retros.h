typedef unsigned char uchar;
typedef unsigned int uint;

enum Change
{
	LH = 0,
	HL=1,
	LL=2,
	HH=3,
	Undef=4,
};

struct Point
{
	float x;
	float y;
};


class RetroDetector
{
public:
#if 0
	float* Discrepancy(
			unsigned char*R0, unsigned char*G0, unsigned char*B0, 
			unsigned char*R1, unsigned char*G1, unsigned char*B1,
			Change R, Change G, Change B,
			int channelsused,
			float &max, float &min
			);
#else
	uint* Discrepancy(
				unsigned char*i0,
				unsigned char*i1,
				Change R, Change G, Change B,
				uint &max, uint &min
				);
#endif
	
	Point* Tapes(
	uchar * i0, uchar * i1,
	int W, int H,
	int &tapes
	);
private:
	uint discrep(uchar _0, uchar _1, Change c);
	
};

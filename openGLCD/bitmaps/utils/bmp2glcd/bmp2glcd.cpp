/*********************************************************************
 *  BMP2GLCD - BMP file to GLCD image header file
 *  
 * vi: ts=4
 *
 *      Purpose: Convert a bitmap to binary bitmap with desired 
 *               dimensions and write a c-header file that contains
 *               the pixel data in 8-bit pages rather than in lines.
 *               This header file can be used for example with 
 *               graphical LCD displays. 
 *    
 *	
 *      License: GPL v3.0 which can be found at
 *               http://www.gnu.org/licenses/gpl.txt
 *
 *   Depends on: EasyBMP library (published under BSD license)
 *
 *    Author(s):
 *		Original Author: Sami J.O. Varjo,
 *		Roberto Oriadne Dalla Valle is acknowledged for 
 *		some debugging and support for PIC30
 *		Bill Perry significant re-write & openGLCD bitmap format support
 *
 *		Version: 2.0
 *			2013-10-05  - bperrybap - Significant updates/re-write & bug fixes
 *                                    Added support for openGLCD
 *
 *      version: 1.02
 *          2008-07-32  - svarjo    - Initial Creation
 *          2009-10-01  - svarjo    - Revised
 *
 *    
 *********************************************************************/

#include <iostream>
#include <fstream>
#include "EasyBMP.h"
#include <unistd.h>
#include <stdlib.h>

#define WRITE_BYTES_PER_LINE 8

#define PROGNAME "bmp2glcd"
#define VERSION  "v2.0"

using namespace std;

const char *UsageStr = "\
Bitmap to GLCD bitmap " VERSION "\n\
Usage: " PROGNAME " [options] <image.bmp>\n\
	-h <height>\tscale target image to height in pixels\n\
	-w <width>\tscale target image to width in pixels\n\
	-v\t\tverbose mode\n\
	-n <name>\toverride basename of header file\n\
	-t <type>\tbitmap header format type\n\
	where <type> is one of:\n\
	C         Create header using standard C const (default, if omitted)\n\
	AVRpgm    create AVR header with __attribute__ ((progmem))\n\
	pic30     create PIC30 header with __attribute__((space(auto_psv)))\n\
	openGLCD  create openGLCD header with GLCDBMAPDECL\n\
\n\
This program converts a bitmap bmp file to binary bitmap (black and white)\n\
with 8bit page height that can be written directly to graphical lcd display.\n\
A C header file is created where data bytes represent pixels in page columns\n\
rather than in lines of pixels.\n\
";

typedef enum
{
	bmt_INVALID,
	bmt_C,
	bmt_AVRpgm,
	bmt_pic30,
	bmt_openGLCD
} bmap_t;
	
typedef struct
{
	const char *str;
	bmap_t type;
} bmapt_lookup;

bmapt_lookup bmtbl[] = {
	{ "C", bmt_C,},
	{ "AVRpgm", bmt_AVRpgm},
	{ "pic30", bmt_pic30},
	{ "openGLCD", bmt_openGLCD},
	{ 0, bmt_INVALID}
}; 

//Forward declarations
void printHelp(void);

bool saveHeaderFile(const char *basename, BMP &image);

bmap_t typeLookUp(const char *type);
const char *typeLookUp(bmap_t type);


//global variables
bool verbose=false;
#define pgm (bmap_type == bmt_AVRpgm)
#define pic30 (bmap_type == bmt_pic30)
#define openGLCD (bmap_type == bmt_openGLCD)

bmap_t bmap_type = bmt_C; // default to C format

const char *pname; 		// name of running program
const char *bmapname;	// name of .bmp file

// error message macro
#define errmsg(fmt, ...) \
	fprintf(stderr, "%s: Error: " fmt "\n", pname, ##__VA_ARGS__)

#define logmsg(fmt, ...) \
	fprintf(stderr, "%s: " fmt "\n", pname, ##__VA_ARGS__)

int main( int argc, char* argv[] )
{
BMP imageIn, imageOutput;
int targetWidth, targetHeight, i;
double coeff;
ifstream test;
int c;
const char *basename = 0;

	pname = argv[0];
  
	if (argc<2)
	{
      printHelp();
      return -2;
	}

	targetWidth=-1;
	targetHeight=-1;

	SetEasyBMPwarningsOff();

	opterr = 0;
	while((c = getopt(argc, argv, ":h:w:vt:n:")) != -1)
	{
		switch(c)
		{
			case 'h':
				targetHeight=atoi(optarg);
				break;
			case 'w':
				targetWidth = atoi(optarg);
				break;
			case 'v':
				verbose = true;
      			SetEasyBMPwarningsOn();
				break;
			case 'n':
				basename = optarg;
				break;
			case 't':
				bmap_type = typeLookUp(optarg);
				if(bmap_type == bmt_INVALID)
				{
					errmsg("%s: Invalid Format type", optarg);
					return(1);
				}
				
				break;

			case '-': 
				printHelp();
				return(1);
				break;

			case ':':
				errmsg("Option -%c requires an argument", optopt);
				return(1);
				break;

			case '?':
				if(optopt == '?')
				{
      				printHelp();
					return(0);
				}
				errmsg("Unknown Option -%c", optopt);
				return(1);
					break;

			default:
				abort();
		}
	}

	bmapname = argv[optind]; // fetch name of .bmp file

	if(!bmapname)
	{
		errmsg("Missing bitmap filename");
		return(1);
	}

	//Check if the file exists
	test.open(bmapname);
	if (test.is_open() != true)
	{
		errmsg("source file: %s can not be openend", bmapname);
		return -1;
	}
	else
	{
		test.close();
	}

	//Read the image in
	try
	{
		imageIn.ReadFromFile(bmapname);
	}
	catch(...)
	{
    	errmsg("source file: %s can not be read", bmapname);
		return -1;
	}    

	if(verbose)
	{
		logmsg("Source height: %d source width:%d",
		 imageIn.TellHeight(), imageIn.TellWidth());
	}

	if(targetHeight>0 && targetWidth<0)
	{
		// scale target width the same as target height
		coeff=(double)targetHeight/imageIn.TellHeight();
		targetWidth=(int)(imageIn.TellWidth()*coeff);
	}
	else if (targetHeight<0 && targetWidth>0)
	{
		// scale target height the same as target width
		coeff=(double)targetWidth/imageIn.TellWidth();
		targetHeight=(int)(imageIn.TellHeight()*coeff);
	}
	else if (targetHeight<0 && targetWidth<0 )
	{
		// no scaling
		targetHeight=imageIn.TellHeight();
		targetWidth=imageIn.TellWidth();
	}
    
	if(targetWidth<targetHeight)
	{
		Rescale( imageIn, 'w' , targetWidth );
	}
	else
	{
		Rescale( imageIn, 'h' , targetHeight );
	}
    
	if(verbose)
		logmsg("Target height: %d target width: %d", targetHeight, targetWidth);

	imageOutput.SetSize( targetWidth , targetHeight );
	RangedPixelToPixelCopy( imageIn, 0, targetWidth-1, targetHeight-1, 0, imageOutput, 0,0);
	imageOutput.SetBitDepth( 1 );    
	CreateGrayscaleColorTable( imageOutput );

	string glcdname(bmapname), outname;
	i=glcdname.find(".bmp",1);
	if(i!=string::npos)
		glcdname = glcdname.replace(i,4,"_glcd");
  
	do // '-' (dash) is not allowed in c header variable name
	{
		i=glcdname.find('-');
		if(i!=string::npos)
			glcdname=glcdname.replace(i,1,"_");
	}while(  i!=string::npos );
 
	outname=glcdname;
	outname+=".bmp";
	if(verbose)
		logmsg("writing a 1bpp image as: \"%s\"", outname.c_str());

	imageOutput.WriteToFile (outname.c_str() );//the pixel data is updated only in write in EasyBMP
	imageOutput.ReadFromFile(outname.c_str() ); 

	// create the .h glcd header file with the data object.
	// unless the basename is overidden with the -n option,
	// the .h basename will be the same as the basename .bmp
	// the data object will also be the same as the basename .bmp
	// i.e. foo.bmp will create foo.h and a data object named foo
	// note: there will also be a 'turd' file created of foo_glcd.bmp
	// which is the converted bitmap
	// (rescaling, and b/w conversion etc...)
	// used to create the glcd bitmap data
	// it is intionally left to provide the bitmap that was used to create the .h data

	if(!basename)
	{
		glcdname.resize(glcdname.size() - 5); // remove "_glcd"
		basename = glcdname.c_str();
	}

	if (!saveHeaderFile(basename, imageOutput))
	{
		errmsg("Error on creating header file");
		return -2;
	}
    
	return 0;
}


//--------------------------------------------------------------------------
//Program commandline help
//
void printHelp()
{
  fprintf(stderr, "%s - %s\n", pname, UsageStr);
}

bool saveHeaderFile(const char *basename, BMP &image)
{
string hdrname(basename);
FILE *fp;
int pages, single_lines, i, j, bi, count;  
int byte; 

	hdrname+=".h";
	fp = fopen(hdrname.c_str(), "w");
	if(!fp)
	{
		errmsg("%s: Cannot open\n", hdrname.c_str());
		return(false);
	}


	pages=(int)image.TellHeight()/8;
	single_lines=image.TellHeight()-pages*8;

	if(verbose)
		logmsg("writing (%s format) header file as: \"%s\"", typeLookUp(bmap_type), hdrname.c_str());
  
	fprintf(fp, "//---------------------------------------------------------------------------\n");
	fprintf(fp, "//    glcd bitmap header created with " PROGNAME " " VERSION "\n");

	if(openGLCD)
	{
		fprintf(fp, "//    The glcd bitmap data contained in this file is in a format\n");
		fprintf(fp, "//    suitable for use by openGLCD.\n");
		fprintf(fp, "//    It contains embedded width and height format information.\n");
	}

	fprintf(fp, "//---------------------------------------------------------------------------\n\n");
	fprintf(fp, "#ifndef _%s_H\n", basename);
	fprintf(fp, "#define _%s_H\n\n", basename);

	if(!openGLCD)
	{
		fprintf(fp, "#define %s_height %d\n", basename, image.TellHeight());
		fprintf(fp, "#define %s_width %d\n", basename, image.TellWidth());
	}

	if (openGLCD)
    	fprintf(fp, "GLCDBMAPDECL(%s) = {\n", basename);
	else if (pgm)
		fprintf(fp, "static unsigned char __attribute__ ((progmem)) %s[] = {\n", basename);
	else if(pic30)
		fprintf(fp, "static unsigned char __attribute__((space(auto_psv))) %s[]={\n", basename);
	else
		fprintf(fp, "const unsigned char %s[]={\n", basename);

	if(openGLCD)
	{
		fprintf(fp, "  %d,\t// width\n", image.TellWidth());
		fprintf(fp, "  %d,\t// height\n", image.TellHeight());
	}

	count = image.TellWidth() * ((image.TellHeight() + 7)/8);

	j=0;
	//whole pages
	while(j<pages)
	{
	    i=0;
		fprintf(fp, "\n  // page %d (lines %d-%d) \n  ", j, j*8, j*8+7);
		while(i<image.TellWidth())
		{
			byte=0x00;
			for(bi=0;bi<8;bi++)
			{	
				// data is already b/w so no need to check all colors
				if((int)image(i,j*8+bi)->Green==0) //pixel is black RGB=(0,0,0)
				{
					byte |= (1<<bi); 
				}
			}

			fprintf(fp, "0x%02x", byte);

			if(--count != 0)
				fputs(",", fp);
   	   
			i++;
			if(i%WRITE_BYTES_PER_LINE==0 && i < image.TellWidth())
				fputs("\n  ", fp);
		}    
		fputc('\n', fp);
		j++;
	}

	if (single_lines>0)
	{
		i=0;
		fprintf(fp, "\n  // partial page %d (lines %d-%d)\n  ", j, j*8, j*8+single_lines-1);
		while(i<image.TellWidth())
		{
			byte=0x00;
			for(bi=0;bi<single_lines;bi++)
			{	
				if((int)image(i,j*8+bi)->Green==0)
				{
					byte |= (1<<bi);
				}
			}

			fprintf(fp, "0x%02x", byte);
            
			if(--count != 0)
				fputs(",", fp);

			i++;      
			if(i%WRITE_BYTES_PER_LINE==0 && i < image.TellWidth())
				fputs("\n  ", fp);
		} 
		fputc('\n',fp);
	}

	fprintf(fp, "};\n");
	fprintf(fp, "#endif  // %s_H\n", basename);
	fflush(fp);
	fclose(fp);

	return true;
}

/*
 * Lookup up type from input string
 */
bmap_t typeLookUp(const char *type)
{
bmapt_lookup *bmtlp = bmtbl;

	while(bmtlp->str)
	{
		if(!strcmp(type, bmtlp->str))
			break;
		bmtlp++;
	}
	return(bmtlp->type);
}
/*
 * Lookup up type string from type enum
 */
const char *typeLookUp(bmap_t type)
{
bmapt_lookup *bmtlp = bmtbl;

	while(bmtlp->str)
	{
		if(type == bmtlp->type)
			break;
		bmtlp++;
	}
	if(bmtlp->type != bmt_INVALID)
		return(bmtlp->str);
	else
		return("INVALID");
}

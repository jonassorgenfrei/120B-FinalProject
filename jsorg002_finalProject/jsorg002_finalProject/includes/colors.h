#ifndef COLOR_H
#define COLOR_H

typedef struct color {
	unsigned char R;
	unsigned char G;
	unsigned char B;
} color;

typedef struct hsv {
	unsigned long hue;
	unsigned char saturation;
	unsigned char value;
} hsv;

double max(double d1, double d2, double d3){
	return (d1 < d2) ? ((d2 < d3)? d3 : d2) : ((d1 < d3)? d3 : d1);	
}

double min(double d1, double d2, double d3){
	return (d1 < d2) ? ((d1 < d3)?d1:d3) : ((d2 < d3) ? d2 : d3);
}

color hsvtorgb(unsigned long hue, unsigned char saturation, unsigned char value)
{
	color tColor;
	double chroma = (double)value/255.0 * (double)saturation/255.0;
	double hue1 = (double)hue/60.0;
	double x = chroma * (double)(1-abs((hue/60)%2-1));
	double r = 0.0,g = 0.0,b = 0.0;
	if(hue1 >= 0 && hue1 <= 1) {
		r = chroma;
		g = x;
		b = 0.0;
	} else if(hue1 >= 1 && hue1 <= 2){
		r = x;
		g = chroma;
		b = 0.0;
	} else if (hue1 >= 2 && hue1 <= 3) {
		r = 0.0;
		g = chroma;
		b = x;
	} else if (hue1 >= 3 && hue1 <= 4) {
		r = 0.0;
		g = x;
		b = chroma;
	} else if (hue1 >= 4 && hue1 <= 5) {
		r = x;
		g = 0.0;
		b = chroma;
	} else if (hue1 >= 5 && hue1 <= 6) {
		r = chroma;
		g = 0.0;
		b = x;
	}
	double m = ((double)value/255) - chroma;
	tColor.R = (unsigned char)((r+m)*255.0);
	tColor.G = (unsigned char)((g+m)*255.0);
	tColor.B = (unsigned char)((b+m)*255.0);
	
	return tColor;
}

hsv rgbtohsv(color rgb){
	hsv tempHSV;
	double r = (double)rgb.R/255.0;
	double g = (double)rgb.G/255.0;
	double b = (double)rgb.B/255.0;	
	
	double cmax = max(r,g,b);
	double cmin = min(r,g,b);
	
	double h;
	double s;
	double v;
	
	v = cmax*255;
	
	double delta = cmax-cmin;
	
	if(cmax > 0){
		s = delta / cmax * 255.0;
	} else {
		tempHSV.hue = 0;
		tempHSV.saturation = 0;
		tempHSV.value = 0;
		return tempHSV;
	}
	
	if(r > (cmax - 0.001) && r < (cmax + 0.001)){
		h = (g - b) / delta;
	} else if(g > (cmax - 0.001) && g < (cmax + 0.001)){
		h = 2.0 + (b - r) / delta;
	} else {
		h = 4 + (r - g) / delta; 
	}
	h *= 60.0;
	if(h < 0){
		h += 360;
	}
	
	tempHSV.hue = (unsigned long)h;
	tempHSV.saturation = (unsigned char)s;
	tempHSV.value = (unsigned char)v;
	
	return tempHSV;
}

#endif
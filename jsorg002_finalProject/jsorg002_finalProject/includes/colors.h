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
	double s = (double)saturation / 255.0f;
	double v = (double)value / 255.0f;
	
	double hue1 = (double)hue;
	if(hue >= 360.0) hue1 = 0.0;
	hue1 /= 60.0f;
	double i = (long)hue1;
	double ff = hue1 - i;
	double p = v * (1.0f - s);
	double q = v * (1.0f - (s*ff));
	double t = v * (1.0f - (s * (1.0f - ff)));
	
	double r = 0.0,g = 0.0,b = 0.0;
	
	if(hue1 >= 0.0f && hue1 < 1.0f) {
		r = v;
		g = t;
		b = p;
	} else if(hue1 >= 1.0f && hue1 < 2.0f){
		r = q;
		g = v;
		b = p;
	} else if (hue1 >= 2.0f && hue1 < 3.0f) {
		r = p;
		g = v;
		b = t;
	} else if (hue1 >= 3.0f && hue1 < 4.0f) {
		r = p;
		g = q;
		b = v;
	} else if (hue1 >= 4.0f && hue1 < 5.0f) {
		r = t;
		g = p;
		b = v;
	} else if (hue1 >= 5.0f && hue1 < 6.0f) {
		r = v;
		g = p;
		b = q;
	}

	tColor.R = (unsigned char)(r*255.0f);
	tColor.G = (unsigned char)(g*255.0f);
	tColor.B = (unsigned char)(b*255.0f);
	
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
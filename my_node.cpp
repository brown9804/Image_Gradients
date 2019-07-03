

// Se incluyó el siguiente header de la biblioteca
// estanda de C para operaciones de entrada y salida
#include <stdio.h>

// Se incluyó el siguiente header de la biblioteca
// estanda de C para gestion de memoria dinamica,
// control de procesos y otras
#include <stdlib.h>

// Se incluyo el siguiente header debido a que usaremos
// funciones matematicas
#include <math.h>

// Se incluyó debido a que se usa la funcion strcpy
#include <string.h>

//***
//***Insertar aqui los prototipos de nuestras funciones
//***

void geoLeerParametrosDeControlDeArchivoDeTexto();
void readRGBImageFromBMPFile(char *filename);
void geoInsertYourCodeHere();
void geoGetIntensityImageFromRGBimage();
void geoDrawALinealSegmentOnIntensityImage();
void geoDrawACircleOnIntensityImage();
void geoSaveIntensityImageIn_YUV400_file(unsigned char *pIntensity, char* filename);
void geoChangeImageCoordinateSystemFromLowerLeftSideToUpperLeftSide(unsigned char *pIntensity, unsigned char *presult);
int SaveIntensityImageIn_BMP_file(unsigned char *pintensity, char *filename);
int SaveRGBImageIn_BMP_file(unsigned char *prgb, char *filename);

void geoGetHistogramAndProbabilityDensityFunctionOfIntensityValues();
void geoGetMeanMeanOfSquaresAndVarianceOfIntensityValues();
void geoGetMeanImage();
void geoGetVariance();
void getXGradient();
void getYGradient();
void magGradientes();
void umbralization();

//***
//***Insertar aqui las definiciones de nuestros contenedores
//***(estructuras)
//***

// La siguiente definicion describe el contenedor que
// usaremos para guardar los parametros de control
// de flujo del programa

//Contenedor de imagenes
struct pInputImage
{
    int width;  // ancho de imagenes
    int height; // alto de imagenes
    unsigned char *prgb; // imagen rgb de entrada
    unsigned char *pintensity; // imagen de intensidad
    unsigned char *pthresholdedIntensity; // imagen resultado
    unsigned char *pdrawnLinealSegmentOnIntensity; // imagen resultado
    unsigned char *pdrawnCircleOnIntensity; // imagen resultado

    int h[256];
    double p[256];

    double ms;								// Valor cuadrático medio
    double med;								// Valor medio
    double var;								// Varianza

    double *pmeanImage;						// Imagen de media
    unsigned char *pmeanImage_uc; 			// Imagen de media uc
    double *pvarianceImage;					// Imagen de varianza
    unsigned char *pvarianceImage_uc;		// Imagen de varianza

    double *pXGradient;						// Double para calcular gradiente en X
    unsigned char *pXGradient_uc;			// Unsigned chart para guardar la imagen de gradientes en x
    double *pYGradient;						// Double para calcular gradiente en Y
    unsigned char *pYGradient_uc;			// Unsigned chart para guardar la imagen de gradientes en y

    double *magnitudGra;					// Double para magnitud de gradientes
    unsigned char *magnitudGra_uc;			// UC para magnitud de gradientes

    double *umbrali;						// Double para imagen binaria (bordes)
    unsigned char *umbrali_uc;				// uc para imagen binaria (bordes)
};

// Contenedor de parametros de control
struct contenedor_de_parametros_de_control
{
    int width; // ancho de las imagenes
    int height; // alto de las imagenes
    char pathAndInputImageFileName[256]; // directorio de entrada
    char pathOfOutputDirectory[256]; // directorio de salida
    int xi; // (xi,yi) punto inicial del segmento lineal
    int yi;
    int xf; // (xf,yf) punto final del segmento lineal
    int yf;
    int cx; // (cx,cy) centro del circulo
    int cy;
    int r; // radio del circulo

    int umbral;			// Umbral
};


//***
//***Insertar aqui las definiciones de variables globales,
//***que son aquellas variables que se podran acceder desde
//***cualquier funcion dentro de este archivo
//***

// El siguiente puntero global apuntara al contenedor que
// usaremos para guardar los valores de control de flujo
// del programa que se leeran de un archivo de texto

struct contenedor_de_parametros_de_control *p_parametros;

// El siguiente puntero global apuntara al contenedor que
// usaremos para guardar las imagenes que utilizaremos
// en el programa

struct pInputImage *pInputImage;

// La siguiente variable global se usara como contador
// el numero de datos leidos

int numeroDeDatosLeidos=0;

//***
//***Insertar aqui las constantes del programa
//***

#define PI 3.141592652

// Inicio de programa principal
int main()
{
    // definición de variables locales
    int i; //contador
        int width, height;

    // Despliegue de autoría en el terminal
    printf("******************************************************************\n");
    printf("** Calculo de imagen de intensidad y dibujo de figuras basicas  **\n");
    printf("** Vision por Computador                                **\n");
    printf("******************************************************************\n");
    printf("\n");

    // Reservando memoria de contenedor p_parametros
    p_parametros = (struct contenedor_de_parametros_de_control *)malloc(sizeof(struct contenedor_de_parametros_de_control));

    // Esta función lee los parámetros de control de flujo del
    // programa desde un archivo de texto y los almacena en el
    // contenedor p_parametros
    geoLeerParametrosDeControlDeArchivoDeTexto();

    // Reservando memoria para la estructura pInputImage
    pInputImage = (struct pInputImage *)malloc(sizeof(struct pInputImage));
    pInputImage->width=p_parametros->width;
    pInputImage->height=p_parametros->height;

    // Reservando e inicializando la memoria de las imagenes del contenedor pInputImage
    width=p_parametros->width;
    height=p_parametros->height;
    pInputImage->prgb = (unsigned char*)malloc(sizeof(unsigned char)*width*height*3);
    pInputImage->pintensity =(unsigned char*)malloc(sizeof(unsigned char)*width*height);
    pInputImage->pthresholdedIntensity =(unsigned char*)malloc(sizeof(unsigned char)*width*height);
    pInputImage->pdrawnLinealSegmentOnIntensity =(unsigned char*)malloc(sizeof(unsigned char)*width*height);
    pInputImage->pdrawnCircleOnIntensity =(unsigned char*)malloc(sizeof(unsigned char)*width*height);
    pInputImage->pmeanImage = (double*)malloc(sizeof(double)*width*height);
    pInputImage->pmeanImage_uc = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
    pInputImage->pvarianceImage = (double*)malloc(sizeof(double)*width*height);
    pInputImage->pvarianceImage_uc = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
    pInputImage->pXGradient = (double*)malloc(sizeof(double)*width*height);
    pInputImage->pXGradient_uc = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
    pInputImage->pYGradient = (double*)malloc(sizeof(double)*width*height);
    pInputImage->pYGradient_uc = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
    pInputImage->magnitudGra = (double*)malloc(sizeof(double)*width*height);
    pInputImage->magnitudGra_uc = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
    pInputImage->umbrali = (double*)malloc(sizeof(double)*width*height);
    pInputImage->umbrali_uc = (unsigned char*)malloc(sizeof(unsigned char)*width*height);


    // Cada píxel se inicializa en cero
    for (i=0;i<width*height*3;i++) pInputImage->prgb[i]=0;
    for (i=0;i<width*height;i++) {
        pInputImage->pintensity[i]=0;
        pInputImage->pthresholdedIntensity[i]=0;
        pInputImage->pdrawnLinealSegmentOnIntensity[i]=0;
        pInputImage->pdrawnCircleOnIntensity[i]=0;
        pInputImage->pmeanImage_uc[i] = 0;
        pInputImage->pmeanImage[i] = 0.0;
        pInputImage->pvarianceImage_uc[i] = 0;
        pInputImage->pvarianceImage[i] = 0.0;
        pInputImage->pXGradient_uc[i] = 0;
        pInputImage->pXGradient[i] = 0.0;
        pInputImage->pYGradient_uc[i] = 0;
        pInputImage->pYGradient[i] = 0.0;
        pInputImage->magnitudGra_uc[i] = 0;
        pInputImage->magnitudGra[i] = 0.0;
        pInputImage->umbrali[i] = 0;
        pInputImage->umbrali_uc[i] = 0.0;
    }

    // Leyendo la imagen RGB de archivo en formato BMP
    readRGBImageFromBMPFile(p_parametros->pathAndInputImageFileName);

    // Insertar codigo en esta funcion
    geoInsertYourCodeHere();

    // Liberando memoria de los contenedores e imagenes
    free(pInputImage->pvarianceImage);
    free(pInputImage->pvarianceImage_uc);
    free(pInputImage->pmeanImage_uc);
    free(pInputImage->pmeanImage);
    free(pInputImage->pXGradient_uc);
    free(pInputImage->pXGradient);
    free(pInputImage->pYGradient_uc);
    free(pInputImage->pYGradient);
    free(pInputImage->magnitudGra_uc);
    free(pInputImage->magnitudGra);
    free(pInputImage->umbrali_uc);
    free(pInputImage->umbrali);
    free(pInputImage->prgb);
    free(pInputImage->pintensity);
    free(pInputImage->pthresholdedIntensity);
    free(pInputImage->pdrawnLinealSegmentOnIntensity);
    free(pInputImage->pdrawnCircleOnIntensity);
    free(pInputImage);
    free(p_parametros);



    return 0;
}
// Fin de programa principal

//*******************************************************
//*******************************************************
//***** Introduzca aqui sus funciones               *****
//*******************************************************
//*******************************************************

// Esta funcion es para insertar nuevo codigo.
void geoInsertYourCodeHere()
{
    int i;
    FILE *text; // Se declara el archivo FILE

    // Abriendo archivo en modo de escritura
    char here[256] = "output/momentos.txt";
    text = fopen(here, "w");
    if (!text) {
        printf("No se pudo abrir el archivo: resultados.txt\n");
        exit(1);
    }

    fprintf(text, "Datos computados:\n\n");

    // Cerrando archivo
    fclose(text);

    // This block saves the readed image
    char pathAndFileName[256];
    strcpy(pathAndFileName,"output/rgb.bmp");
    SaveRGBImageIn_BMP_file(pInputImage->prgb, pathAndFileName);


    // Calculando la imagen de intensidad
    geoGetIntensityImageFromRGBimage();

    // Almacenando resultado en archivo en formato YUV400
    // strcpy(pathAndFileName,"output/intensity.yuv");
    // geoSaveIntensityImageIn_YUV400_file(pInputImage->pintensity, pathAndFileName);
    strcpy(pathAndFileName,"output/intensity.bmp");
    SaveIntensityImageIn_BMP_file(pInputImage->pintensity, pathAndFileName);

    // Dibujando segmento lineal sobre imagen de intensidad
    //geoDrawALinealSegmentOnIntensityImage();
    //Almacenando resultado en archivo en formato YUV400
    //strcpy(pathAndFileName,"output/linearSegment.yuv");
    //geoSaveIntensityImageIn_YUV400_file(pInputImage->pdrawnLinealSegmentOnIntensity, pathAndFileName);
    //strcpy(pathAndFileName,"output/linearSegment.bmp");
    //SaveIntensityImageIn_BMP_file(pInputImage->pdrawnLinealSegmentOnIntensity, pathAndFileName);

    // Dibujando circulo sobre imagen de intensidad
    //geoDrawACircleOnIntensityImage();
    //Almacenando resultado en archivo en formato YUV400
    //strcpy(pathAndFileName,"output/circle.yuv");
    //geoSaveIntensityImageIn_YUV400_file(pInputImage->pdrawnCircleOnIntensity, pathAndFileName);
    //strcpy(pathAndFileName,"output/circle.bmp");
    //SaveIntensityImageIn_BMP_file(pInputImage->pdrawnCircleOnIntensity, pathAndFileName);


    //Calculando Histograma y densidad de probabilidad
    geoGetHistogramAndProbabilityDensityFunctionOfIntensityValues();

    //Calculando valor medio cuadrado y varianza
    geoGetMeanMeanOfSquaresAndVarianceOfIntensityValues();

    //Calculando la imagen media
    geoGetMeanImage();

    // Salvando la imagen de valores medios en formato bmp
    strcpy(pathAndFileName,"output/imagenDeMediasLocales.bmp");
    SaveIntensityImageIn_BMP_file(pInputImage->pmeanImage_uc, pathAndFileName);

    // Obteniendo la imagen de varianzas locales de intensidad
    geoGetVariance();

    // Salvando la imagen de varianza local de intensidad bmp
    strcpy(pathAndFileName,"output/imagenDeVarianzasLocales.bmp");
    SaveIntensityImageIn_BMP_file(pInputImage->pvarianceImage_uc, pathAndFileName);

    // Calculando gradiente en X
    getXGradient();

    // Salvando la imagen de gradientes X en formato bmp
    strcpy(pathAndFileName,"output/gradienteSobelX.bmp");
    SaveIntensityImageIn_BMP_file(pInputImage->pXGradient_uc, pathAndFileName);

    // Calculando gradiente en Y
    getYGradient();

    // Salvando la imagen de gradientes X en formato bmp
    strcpy(pathAndFileName,"output/gradienteSobelY.bmp");
    SaveIntensityImageIn_BMP_file(pInputImage->pYGradient_uc, pathAndFileName);

    // Calculando magnitud de gradiente
    magGradientes();

    // Salvando la imagen de magnitud de gradientes en formato bmp
    strcpy(pathAndFileName,"output/magGradientes.bmp");
    SaveIntensityImageIn_BMP_file(pInputImage->magnitudGra_uc, pathAndFileName);

    // Umbralizando la imágen
    umbralization();
    // Salvando la imagen de bordes en formato bmp
    strcpy(pathAndFileName,"output/imagenDeBordes.bmp");
    SaveIntensityImageIn_BMP_file(pInputImage->umbrali_uc, pathAndFileName);


}


void umbralization()
{
	int h, w, i, y, x, c, u;
	c = 0;

	int *bord; 							// Allow us to build borders

	double *g;							// Contains Magnitude of Grad
	g = pInputImage->magnitudGra;
	u = p_parametros->umbral;			// Is the given umbral
    h = pInputImage->height;
    w = pInputImage->width;

    // Allocating memory for bord
	bord = (int *)malloc(sizeof(int)*w*h);
	for (i = 0; i < h*w; i++)
	{
		bord[i] = 0;					// Inicializando bord
	}

	for (y=1; y<(h-1); y++)
	{
		for (x=1; x<(w-1); x++)
		{
			if (g[y*w+x]>=u)			// Si la magnitud de gradiente es mayor que el umbral
			{
				bord[y*w+x]=255;		// Asignar color blanco
				c = c+1;
			}
			else
			{
				bord[y*w+x]=0;			// De otro modo, asignarlo negro
			}
		}
	}

	// Saving result
	for (i = 0; i < w*h; i++)
	{
		pInputImage->umbrali[i] = (double)bord[i];
		pInputImage->umbrali_uc[i] = (unsigned char)bord[i];
	}

	// Freeing memory
	free(bord);

	printf("     La cantidad de pixeles que conforman bordes son: %i\n", c);

}




void magGradientes()
{
	int h, w, i, y, x;
	double *g, *gx, *gy;

	gx = pInputImage->pXGradient;
	gy =  pInputImage->pYGradient;

    h = pInputImage->height;
    w = pInputImage->width;

    // Allocating memory for g image
	g = (double *)malloc(sizeof(double)*w*h);

	for (i = 0; i < h*w; i++)
	{
		g[i] = 0.0;
	}

	for (y=1; y<(h-1); y++)
	{
		for (x=1; x<(w-1); x++)
		{

			g[y*w+x] = sqrt( (double)(gx[y*w+x])*(double)(gx[y*w+x]) + (double)(gy[y*w+x])*(double)(gy[y*w+x])  );
		}
	}


	// Saving result
	for (i = 0; i < w*h; i++)
	{
		pInputImage->magnitudGra[i] = g[i];
	}
		for (i = 0; i < w*h; i++)
	{
		if (fabs(g[i])>=255.0)
		{
			pInputImage->magnitudGra_uc[i] = 255.0;
		}
		else
		{
			pInputImage->magnitudGra_uc[i] = fabs(g[i]);
		}
	}


	// Freeing memory
	free(g);



}


void getYGradient()
{
	double *pgy;
	int y, x, w, h, i;
	unsigned char *pY;
	pY = pInputImage->pintensity;

    h = pInputImage->height;
    w = pInputImage->width;

	// Allocating memory for pgy image
	pgy = (double *)malloc(sizeof(double)*w*h);
	for (i = 0; i < h*w; i++)
	{
		pgy[i] = 0.0;
	}

	// Getting Y gradients
	for (y = 1; y<h-1 ; y++)
	{
		for (x = 1; x < w-1; x++)
		{
			pgy[y*w+x]= (1.0/8.0)*((double)pY[(y+1)*w+(x-1)] +
						      +2.0*(double)pY[(y+1)*w+x]     +
						          +(double)pY[(y+1)*w+(x+1)] +
						          -(double)pY[(y-1)*w+(x-1)] +
						      -2.0*(double)pY[(y-1)*w+x]     +
						          -(double)pY[(y-1)*w+(x+1)]);

		}
	}

	// Saving result
	for (i = 0; i < w*h; i++)
	{
		pInputImage->pYGradient[i] = pgy[i];
	}
		for (i = 0; i < w*h; i++)
	{
		if (fabs(pgy[i])>=255.0)
		{
			pInputImage->pYGradient_uc[i] = 255;
		}
		else
		{
			pInputImage->pYGradient_uc[i] = fabs(pgy[i]);
		}
	}

	// Freeing memory
	free(pgy);

}




void getXGradient()
{
	double *pgx;
	int y, x, w, h, i;
	unsigned char *pY;
	pY = pInputImage->pintensity;

    h = pInputImage->height;
    w = pInputImage->width;

	// Allocating memory for pgx image
	pgx = (double *)malloc(sizeof(double)*w*h);
	for (i = 0; i < h*w; i++)
	{
		pgx[i] = 0.0;
	}

	// Getting X gradients
	for (y = 1; y<h-1 ; y++)
	{
		for (x = 1; x < w-1; x++)
		{
			pgx[y*w+x]= (double)(1.0/8.0)*((double)pY[(y+1)*w+(x+1)] +
						              +2.0*(double)pY[y*w+(x+1)]     +
						                  +(double)pY[(y-1)*w+(x+1)] +
						                  -(double)pY[(y+1)*w+(x-1)] +
						              -2.0*(double)pY[y*w+(x-1)]     +
						                  -(double)pY[(y-1)*w+(x-1)]);

		}
	}

	// Saving result
	for (i = 0; i < w*h; i++)
	{
		pInputImage->pXGradient[i] = pgx[i];
	}
		for (i = 0; i < w*h; i++)
	{
		if (fabs(pgx[i])>=255.0)
		{
			pInputImage->pXGradient_uc[i]=255;
		}
		else
		{
			pInputImage->pXGradient_uc[i] = (unsigned char)fabs(pgx[i]);
		}

	}

	// Freeing memory
	free(pgx);

}

void geoGetVariance()
{
    double *ptempImage, *pm;
    unsigned char *pi;
    int x, y, w, h, i, W, C, n, m;

    h = pInputImage->height;
    w = pInputImage->width;
    pi = pInputImage->pintensity;
    pm = pInputImage->pmeanImage;

    // Allocating memory for temporal image
    ptempImage = (double*)malloc(sizeof(double)*w*h);
    for ( i = 0; i < h*w; ++i)
        {
            ptempImage[i]=0.0;
        }

        // Getting the variance image
        W=1;
        C=(2*W+1)*(2*W+1)-1;

        for (y = W; y < (h-W); y++)
        {
            for (x = W; x < (w-W);  x++)
            {
                for (n=-W; n<=W; n++)
                {
                    for (m=-W; m<=W; m++)
                    {
                        ptempImage[y*w+x] = ptempImage[y*w+x]+
                        (double)pi[(y+n)*w+x+m]*(double)pi[(y+n)*w+x+m]-
                        pm[y*w+x]*pm[y*w+x];
                    }
                }

                ptempImage[y*w+x] = ptempImage[y*w+x]/(double)C;
            }
        }



        // Saving result
        for (i = 0; i < h*w; i++)
        {
            pInputImage->pvarianceImage[i]=ptempImage[i];
            pInputImage->pvarianceImage_uc[i] = (unsigned char)ptempImage[i];
        }

    free(ptempImage);


}

void geoGetHistogramAndProbabilityDensityFunctionOfIntensityValues()
{
    int w, h, i, N, intensityValue, his[256];
    double p[256];
    h = pInputImage->height;
    w = pInputImage->width;
    N = h*w;

    //Initialization
    for (i = 0;i < 256; i++)
    {
        his[i] = 0;
        p[i] = 0.0;
    }

    //Getting the histogram of the intensity values
    for(i = 0;i < w*h; i++)
    {
        intensityValue = pInputImage->pintensity[i];
        his[intensityValue] = his[intensityValue]+1;
    }

    printf("\nLos histogramas son:\n");

    for(i = 0;i < 256; i++)
    {
        printf("     Histograma[%d]:     %d\n", i , his[i]);
    }

    //Getting the probability density function of the intensity values
    for(i=0; i<256 ;i++)
    {
        p[i]=(double)his[i]/(double)N;
    }

    printf("\nLa densidad de probabilidad es:\n");

    for(i=0;i<256;i++)printf("     P[%d]:      [%f]\n", i, p[i]);

    //saving results
    for(i=0;i<256;i++)
    {
        pInputImage->h[i]=his[i];
        pInputImage->p[i]=p[i];
    }

    FILE *text;

    //Abriendo archivo en modo de escritura
    char here[256] = "output/momentos.txt";
    text = fopen(here, "at");
    if (!text) {
        printf("No se pudo abrir el archivo: resultados.txt\n");
        exit(1);
    }

    fprintf(text,"\nLos histogramas son:\n");

    for(i = 0;i < 256; i++)
    {
        fprintf(text,"     Histograma[%d]:     %d\n", i , his[i]);
    }
    //Cerrando archivo
    fclose(text);
}

void geoGetMeanMeanOfSquaresAndVarianceOfIntensityValues()
{
    int i;
    double m, ms, var;

    //Getting the mean of intensity values
    m=0.0;
    for(i=0;i<256;i++){
    m = m+(double)i*pInputImage->p[i];
    }
    printf("\n     El valor medio es: [%f]\n", m);

    //Getting the mean of squares of the intensity values
    ms=0.0;
    for(i=0;i<256;i++){
        ms=ms+(double)(i*i)*pInputImage->p[i];
    }
    printf("\n     El valor medio cuadrático es: [%f]\n", ms);

    //Getting the variance of the intensity values
    var = 0.0;
    for(i=0;i<256;i++){
        var=var+((double)i-m)*((double)i-m)*pInputImage->p[i];
    }
    printf("\n     La varianza es: [%f]\n",var);

    pInputImage->ms=ms;
    pInputImage->med=m;
    pInputImage->var=var;

    FILE *text;

    //Abriendo archivo en modo de escritura
    char here[256] = "output/momentos.txt";
    text = fopen(here, "at");
    if (!text) {
        printf("No se pudo abrir el archivo: resultados.txt\n");
        exit(1);
    }

    fprintf(text,"\n     El valor medio es: [%f]\n", m);
    fprintf(text,"\n     El valor medio cuadrático es: [%f]\n", ms);
    fprintf(text,"\n     La varianza es: [%f]\n",var);

    //Cerrando archivo
    fclose(text);
}

void geoGetMeanImage()
{
    double *ptempImage, sum;
    unsigned char *pi;
    int x,y,w,h,i,W,C,n,m;

    h=pInputImage->height;
    w=pInputImage->width;
    pi=pInputImage->pintensity;

    //Allocating memory fot tempral image
    ptempImage = (double *)malloc(sizeof(double)*w*h);
    for(i=0;i<h*w;i++)ptempImage[i]=0.0;

    //Getting the mean image
    W=1;
    C=(2*W+1)*(2*W+1);

    for(y=W;y<(h-W);y++){
        for(x=W;x<(w-W);x++){
            sum =0.0;
            for(n=-W;n<=W;n++){
                for(m=-W;m<=W;m++){
                    sum = sum + (double)pi[(y+n)*w+x+m];
                }
            }
            ptempImage[y*w+x]=sum/(double)C;
        }
    }

    //saving result
    for (i=0;i<h*w;i++)
        {
            pInputImage->pmeanImage[i] = ptempImage[i];
            pInputImage->pmeanImage_uc[i] = (unsigned char)ptempImage[i];
        }
//Freeing memory
free(ptempImage);
}




//Esta funcion obtiene la imagen de intensidad de
//una imagen RGB
void geoGetIntensityImageFromRGBimage()
{
    int i,j;
    int width, height;

    //Renombrando para facilitar código
    width= pInputImage->width;
    height=pInputImage->height;

    //Calculando la imagen de intensidad. El resultado será almacenado
    //en el espacio que fue alocado para tal fin en nuestra estructura
    //pInputImage
    for (j=0;j<height; j++) {
        for (i=0;i<width; i++) {
            pInputImage->pintensity[j*width+i] =
            (unsigned char)(0.299*(double)pInputImage->prgb[3*j*width+3*i]+
                            0.587*(double)pInputImage->prgb[3*j*width+3*i+1]+
                            0.114*(double)pInputImage->prgb[3*j*width+3*i+2]);
        }
    }
}

//Esta funcion dibuja un segmento lineal sobre
//la imagen de intensidad
void geoDrawALinealSegmentOnIntensityImage()
{
    int x, y, i;
    double alfa;
    int xi, yi, xf, yf;
    int height, width;

    xi=p_parametros->xi;
    yi=p_parametros->yi;
    xf=p_parametros->xf;
    yf=p_parametros->yf;

    height=pInputImage->height;
    width=pInputImage->width;

    //Copiando la imagen de intensidad en la imagen que contendrá el segmento lineal
    for (i=0;i<pInputImage->height*pInputImage->width;i++)
        pInputImage->pdrawnLinealSegmentOnIntensity[i]=pInputImage->pintensity[i];

    //Por cada valor del parámetro alfa se calcula un punto usando las ecuaciones
    //paramétricas de un segmento lineal. Cada punto se pone en 255 (blanco) en la
    //imagen de salida. Alfa varía entre 0.0 y 1.0 en pasos de GEO_ALFA_STEP. Cuando
    //alfa en 0.0 se estaría en la posición inicial del segmento y cuando alfa es
    //1.0 en el punto final del segmento.
    for (alfa=0.0;alfa<=1.0;alfa=alfa+0.001) {
        //x=xi+alfa*(xf-xi)
        x=(int)((double)xi+alfa*((double)xf-(double)xi));
        //y=iy+alfa*(fy-iy)
        y=(int)((double)yi+alfa*((double)yf-(double)yi));

        //Dibujando el punto (x,y) siempre y cuando no esté fuera de la imagen
        if ((y>=0)&&(x>=0)&&(y<height)&&(x<width)) {
            pInputImage->pdrawnLinealSegmentOnIntensity[y*width+x]=255;
        }
    }
}

//Esta funcion dibuja un circulo sobre la imagen de
//intensidad
void geoDrawACircleOnIntensityImage()
{
    int x, y, i;
    double angle;
    int cx, cy, r;
    int height, width;

    cx=p_parametros->cx;
    cy=p_parametros->cy;
    r=p_parametros->r;

    height=pInputImage->height;
    width=pInputImage->width;

    //Copiando la imagen de intensidad en la imagen que contendrá
    //el círculo
    for (i=0;i<height*width;i++)
        pInputImage->pdrawnCircleOnIntensity[i]=pInputImage->pintensity[i];

    //Por cada valor del parámetro angle se calcula un punto usando las ecuaciones
    //paramétricas de un círculo. Cada punto se pone en 255 (blanco) en la
    //imagen de salida. angle varía entre 0.0 y 2*PI en pasos de 0.001. Cuando
    //angle en 0.0 se estaría en la posición inicial del círculo (a una distancia
    //radius sobre el eje horizontal del círculo) y cuando angle es 2*PI ya
    //habríamos dado la vuelta y estaríamos cerrando el círculo
    for (angle=0.0;angle<2.0*(double)PI;angle=angle+0.001) {
        //x=cx+r*cos(angle)
        x=(int)((double)cx+(double)r*cos(angle));
        //y=cy+r*sin(angle)
        y=(int)((double)cy+(double)r*sin(angle));

        //Dibujando el punto (x,y) siempre y cuando no esté fuera de la imagen
        if ((y>=0)&&(x>=0)&&(y<height)&&(x<width)) {
            pInputImage->pdrawnCircleOnIntensity[y*width+x]=255;
        }
    }
}

//Esta funcion lee los parametros de archivo de parametros
//de control
void geoLeerParametrosDeControlDeArchivoDeTexto()
{
    FILE *archivo;
    char d1[256], d2[256];
    int res;

    printf("Leyendo los datos de entrada:\n");

    //Abriendo archivo en mode de lectura
    char nombreDeArchivo[256]="current_control_parameters.txt";
    archivo = fopen(nombreDeArchivo, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo: current_control_parameters.txt\n");
        exit(1);
    }

    //Leyendo datos linea por linea

    //Leyendo path
    res=fscanf(archivo, "%s %s\n", d1, d2);
    if (res==0) {printf("Error leyendo dato No. %d de archivo de parametros de control\n", numeroDeDatosLeidos); exit(0);}
    strcpy(p_parametros->pathAndInputImageFileName,d2);
    printf("        Entrada:    %s\n", p_parametros->pathAndInputImageFileName);
    numeroDeDatosLeidos++;

    //Leyendo width
    res=fscanf(archivo, "%s %s\n", d1, d2);
    if (res==0) {printf("Error leyendo dato No. %d de archivo de parametros de control\n", numeroDeDatosLeidos); exit(0);}
    p_parametros->width=(int)atof(d2);
    printf("        width:      %d\n", p_parametros->width);
    numeroDeDatosLeidos++;

    //Leyendo height
    res=fscanf(archivo, "%s %s\n", d1, d2);
    if (res==0) {printf("Error leyendo dato No. %d de archivo de parametros de control\n", numeroDeDatosLeidos); exit(0);}
    p_parametros->height=(int)atof(d2);
    printf("        height:     %d\n", p_parametros->height);
    numeroDeDatosLeidos++;

    //Leyendo umbral
    res=fscanf(archivo, "%s %s\n", d1, d2);
    if (res==0) {printf("Error leyendo dato No. %d de archivo de parametros de control\n", numeroDeDatosLeidos); exit(0);}
    p_parametros->umbral=(int)atof(d2);
    printf("        Umbral:     %d\n", p_parametros->umbral);
    numeroDeDatosLeidos++;

    //Leyendo directorio de salida
    res=fscanf(archivo, "%s %s\n", d1, d2);
    if (res==0) {printf("Error leyendo dato No. %d de archivo de parametros de control\n", numeroDeDatosLeidos); exit(0);}
    strcpy(p_parametros->pathOfOutputDirectory,d2);
    printf("        Salida:     %s\n", p_parametros->pathOfOutputDirectory);
    numeroDeDatosLeidos++;

    //Cerrando archivo
    fclose(archivo);
}

//****************************************
//****************************************
//** FUNCIONES DE LECTURA Y ESCRITURA   **
//** DE IMAGENES                        **
//****************************************
//****************************************

struct BMPHeader
{
    char bfType[3];       /* "BM" */
    int bfSize;           /* Size of file in bytes */
    int bfReserved;       /* set to 0 */
    int bfOffBits;        /* Byte offset to actual bitmap data (= 54) */
    int biSize;           /* Size of BITMAPINFOHEADER, in bytes (= 40) */
    int biWidth;          /* Width of image, in pixels */
    int biHeight;         /* Height of images, in pixels */
    short biPlanes;       /* Number of planes in target device (set to 1) */
    short biBitCount;     /* Bits per pixel (24 in this case) */
    int biCompression;    /* Type of compression (0 if no compression) */
    int biSizeImage;      /* Image size, in bytes (0 if no compression) */
    int biXPelsPerMeter;  /* Resolution in pixels/meter of display device */
    int biYPelsPerMeter;  /* Resolution in pixels/meter of display device */
    int biClrUsed;        /* Number of colors in the color table (if 0, use
                           maximum allowed by biBitCount) */
    int biClrImportant;   /* Number of important colors.  If 0, all colors
                           are important */
};

//Esta funcion lee imagen RGB de archivo en formato BMP
void readRGBImageFromBMPFile(char *filename)
{
    FILE *fd;
    int width, height;
    int i, j;
    int n;

    fd = fopen(filename, "rb");
    if (fd == NULL)
    {
        printf("Error: fopen failed\n");
        return;
    }

    unsigned char header[54];

    // Read header
    n=fread(header, sizeof(unsigned char), 54, fd);
    if (n==0) {printf("No se pudieron leer datos\n"); exit(0);}

    // Capture dimensions
    width = *(int*)&header[18];
    height = *(int*)&header[22];

    int padding = 0;

    // Calculate padding
    while ((width * 3 + padding) % 4 != 0)
    {
        padding++;
    }

    // Compute new width, which includes padding
    int widthnew = width * 3 + padding;

    // Allocate temporary memory to read widthnew size of data
    unsigned char* data = (unsigned char *)malloc(widthnew * sizeof (unsigned int));

    // Read row by row of data and remove padded data.
    for (i = 0; i<height; i++)
    {
        // Read widthnew length of data
        n=fread(data, sizeof(unsigned char), widthnew, fd);
        if (n==0) {printf("No se pudieron leer datos\n"); exit(0);}

        // Retain width length of data, and swizzle RB component.
        // BMP stores in BGR format, my usecase needs RGB format
        for (j = 0; j < width * 3; j += 3)
        {
            int index = (i * width * 3) + (j);
            pInputImage->prgb[index + 0] = data[j + 2];
            pInputImage->prgb[index + 1] = data[j + 1];
            pInputImage->prgb[index + 2] = data[j + 0];
        }
    }

    free(data);
    fclose(fd);
}

//Esta funcion almacena una imagen de intensidad en
//archivo en formato YUV400
void geoSaveIntensityImageIn_YUV400_file(unsigned char *pintensity, char* filename)
{
    int numread, j, jj, i;
    FILE *fpointer;
    int width, height;
    unsigned char *ptempImage;

    width=pInputImage->width;
    height=pInputImage->height;

    //Imagen de uso local y temporal
    ptempImage = (unsigned char *)malloc(sizeof(unsigned char)*width*height);

    printf("Saving unsigned char image in: %s\n", filename);

    //Abriendo archivo
    fpointer = fopen(filename, "w");
    if (fpointer == NULL) {
        printf("could not read the file: %s\n", filename);
        exit(0);
    }

    //Cambiando posición del sistema de coordenadas de la equina inferior
    //izquierda a la esquina superior izquierda.
    for (i=0;i<width*height;i++) ptempImage[i]=0;
    jj=0;
    for (j=height-1;j>=0;j--) {
        for (i=0;i<width;i++) {
            ptempImage[jj*width+i]= pintensity[j*width+i];
        }
        jj++;
    }

    //Almacenando valores de intensidad en archivo
    numread = (int)fwrite(ptempImage, sizeof(unsigned char), (unsigned int)(height*width), fpointer);

    if (numread==0) {
        printf("Por alguna razon no se pudo escribir dato alguno en archivo\n");
        exit(0);
    }

    fclose(fpointer);
    free(ptempImage);
}

//Esta funcion almacena una imagen de intensidad en
//archivo en formato BMP
int SaveIntensityImageIn_BMP_file(unsigned char *pintensity, char *filename)
{
    int i, j, jj, ipos;
    int bytesPerLine;
    unsigned char *line;
    unsigned char *ptempImage;
    int height, width;

    height=pInputImage->height;
    width=pInputImage->width;

    FILE *file;
    struct BMPHeader bmph;

    /* The length of each line must be a multiple of 4 bytes */

    bytesPerLine = (3 * (width + 1) / 4) * 4;

    strcpy(bmph.bfType, "BM");
    bmph.bfOffBits = 54;
    bmph.bfSize = bmph.bfOffBits + bytesPerLine * height;
    bmph.bfReserved = 0;
    bmph.biSize = 40;
    bmph.biWidth = width;
    bmph.biHeight = height;
    bmph.biPlanes = 1;
    bmph.biBitCount = 24;
    bmph.biCompression = 0;
    bmph.biSizeImage = bytesPerLine * height;
    bmph.biXPelsPerMeter = 0;
    bmph.biYPelsPerMeter = 0;
    bmph.biClrUsed = 0;
    bmph.biClrImportant = 0;

    file = fopen (filename, "wb");
    if (file == NULL) return(0);

    fwrite(&bmph.bfType, 2, 1, file);
    fwrite(&bmph.bfSize, 4, 1, file);
    fwrite(&bmph.bfReserved, 4, 1, file);
    fwrite(&bmph.bfOffBits, 4, 1, file);
    fwrite(&bmph.biSize, 4, 1, file);
    fwrite(&bmph.biWidth, 4, 1, file);
    fwrite(&bmph.biHeight, 4, 1, file);
    fwrite(&bmph.biPlanes, 2, 1, file);
    fwrite(&bmph.biBitCount, 2, 1, file);
    fwrite(&bmph.biCompression, 4, 1, file);
    fwrite(&bmph.biSizeImage, 4, 1, file);
    fwrite(&bmph.biXPelsPerMeter, 4, 1, file);
    fwrite(&bmph.biYPelsPerMeter, 4, 1, file);
    fwrite(&bmph.biClrUsed, 4, 1, file);
    fwrite(&bmph.biClrImportant, 4, 1, file);

    line = (unsigned char*) malloc(bytesPerLine);
    if (line == NULL)
    {
        fprintf(stderr, "Can't allocate memory for BMP file.\n");
        return(0);
    }

    //Cambiando posición del sistema de coordenadas de la equina inferior
    //izquierda a la esquina superior izquierda.
    ptempImage = (unsigned char *)malloc(sizeof(unsigned char)*width*height);
    for (i=0;i<width*height;i++) ptempImage[i]=0;
    jj=0;
    for (j=height-1;j>=0;j--) {
        for (i=0;i<width;i++) {
            ptempImage[jj*width+i]= pintensity[j*width+i];
        }
        jj++;
    }

    for (i = height - 1; i >= 0; i--)
    {
        for (j = 0; j < width; j++)
        {
            ipos = (width * i + j);
            line[3*j] = ptempImage[ipos];
            line[3*j+1] = ptempImage[ipos];
            line[3*j+2] = ptempImage[ipos];
        }
        fwrite(line, bytesPerLine, 1, file);
    }

    free(line);
    fclose(file);

    free(ptempImage);

    return(1);
}

//Esta funcion almacena una imagen RGB en
//archivo en formato BMP
int SaveRGBImageIn_BMP_file(unsigned char *prgb, char *filename)
{
    int i, j, jj, ipos;
    int bytesPerLine;
    unsigned char *line;
    unsigned char *ptempImage;
    int height, width;

    height=pInputImage->height;
    width=pInputImage->width;

    FILE *file;
    struct BMPHeader bmph;

    /* The length of each line must be a multiple of 4 bytes */

    bytesPerLine = (3 * (width + 1) / 4) * 4;

    strcpy(bmph.bfType, "BM");
    bmph.bfOffBits = 54;
    bmph.bfSize = bmph.bfOffBits + bytesPerLine * height;
    bmph.bfReserved = 0;
    bmph.biSize = 40;
    bmph.biWidth = width;
    bmph.biHeight = height;
    bmph.biPlanes = 1;
    bmph.biBitCount = 24;
    bmph.biCompression = 0;
    bmph.biSizeImage = bytesPerLine * height;
    bmph.biXPelsPerMeter = 0;
    bmph.biYPelsPerMeter = 0;
    bmph.biClrUsed = 0;
    bmph.biClrImportant = 0;

    file = fopen (filename, "wb");
    if (file == NULL) return(0);

    fwrite(&bmph.bfType, 2, 1, file);
    fwrite(&bmph.bfSize, 4, 1, file);
    fwrite(&bmph.bfReserved, 4, 1, file);
    fwrite(&bmph.bfOffBits, 4, 1, file);
    fwrite(&bmph.biSize, 4, 1, file);
    fwrite(&bmph.biWidth, 4, 1, file);
    fwrite(&bmph.biHeight, 4, 1, file);
    fwrite(&bmph.biPlanes, 2, 1, file);
    fwrite(&bmph.biBitCount, 2, 1, file);
    fwrite(&bmph.biCompression, 4, 1, file);
    fwrite(&bmph.biSizeImage, 4, 1, file);
    fwrite(&bmph.biXPelsPerMeter, 4, 1, file);
    fwrite(&bmph.biYPelsPerMeter, 4, 1, file);
    fwrite(&bmph.biClrUsed, 4, 1, file);
    fwrite(&bmph.biClrImportant, 4, 1, file);

    line = (unsigned char*) malloc(bytesPerLine);
    if (line == NULL)
    {
        fprintf(stderr, "Can't allocate memory for BMP file.\n");
        return(0);
    }

    //Cambiando posición del sistema de coordenadas de la equina inferior
    //izquierda a la esquina superior izquierda.
    ptempImage = (unsigned char *)malloc(sizeof(unsigned char)*width*height*3);
    for (i=0;i<width*height*3;i++) ptempImage[i]=0;
    jj=0;
    for (j=height-1;j>=0;j--) {
        for (i=0;i<width;i++) {
            ptempImage[jj*3*width+3*i]= prgb[j*3*width+3*i];
            ptempImage[jj*3*width+3*i+1]= prgb[j*3*width+3*i+1];
            ptempImage[jj*3*width+3*i+2]= prgb[j*3*width+3*i+2];
        }
        jj++;
    }

    for (i = height - 1; i >= 0; i--)
    {
        for (j = 0; j < width; j++)
        {
            ipos = 3*(width * i + j);
            line[3*j] = ptempImage[ipos+2];
            line[3*j+1] = ptempImage[ipos+1];
            line[3*j+2] = ptempImage[ipos];
        }
        fwrite(line, bytesPerLine, 1, file);
    }

    free(line);
    fclose(file);

    free(ptempImage);

    return(1);
}

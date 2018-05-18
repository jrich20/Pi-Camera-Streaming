//  Created by Marcelo Ragone on 19 Feb 2017.
//  v 0.2


#include <stdio.h>
#include <stdlib.h>                                         // malloc()
#include <string.h>                                         // strcmp(), strlen(), strncpy(), strcat(), strlen()

// cámara RP_imx219
#define wdata               4128                            // ancho en bytes de la matriz RAW de captura (4 valores 10 bits cada 5 bytes)
#define hdata               2480                            // alto
#define wrgb                1640                            // ancho de la matriz RGB de salida (max resolución horizontal / 2)
#define hrgb                1232                            // alto (idem vertical)

#define offset          10270208                            // lugar de la palabra magica: "BRCM"

int main(int argc, const char * argv[]) {
    if (argc != 2) {
        printf("raw2ppm usage: raw2ppm filename\nWill generate a filename.ppm image from the original RPi_imx219 raw file\n");
        return 1;
    }

    // genero un array para la data RAW en memoria
    unsigned char* data = malloc(wdata * hdata);            if (data == NULL) return 1;

    // abro un archivo y cargo la data
    FILE* file = fopen(argv[1], "rb");                      if (!file) return 2;

    // verifico que sea un archivo compatible
    fseek(file, - offset, SEEK_END);                        // posiciono al principio de la data que interesa desde el final para atras
    char str[] = "1234";
    fread(str, 1, 4, file);                                 // leo 4 letras
    if (strcmp(str, "BRCM") != 0) {
        printf("raw2ppm: Unsupported file\n");
        return 3;
    }

    fseek(file, - wdata * hdata, SEEK_END);                 // posiciono al principio de la data que interesa desde el final para atras
    fread(data, 1, wdata * hdata, file);                    // leo el archivo y lo pongo en la memoria
    fclose(file);

    // genero un array rgb en memoria y lo lleno
    unsigned char* rgb = malloc(wrgb * hrgb * 3);           if (rgb == NULL) return 4;

    for (int x = 0; x < wrgb; x++) {
        int xdata = (x >> 1) * 5 + ( (x << 1) & 0b11 );     // xdata = (xword // 4) * 5 + (xword & 0b11), xword = 2 * xrgb

        for (int y = 0; y < hrgb; y++) {
            int ydata = y << 1;                             // ydata = yword = 2 * yrgb

            rgb[ (y * wrgb + x) * 3 + 0 ] = data[ (ydata    ) * wdata + (xdata + 1) ];          // R
            rgb[ (y * wrgb + x) * 3 + 1 ] =(data[ (ydata    ) * wdata + (xdata    ) ] >> 1) + (data[ (ydata + 1) * wdata + (xdata + 1) ] >> 1);  // (G1 + G2) / 2
            rgb[ (y * wrgb + x) * 3 + 2 ] = data[ (ydata + 1) * wdata + (xdata    ) ];          // B
        }
    }

    // paso el array rgb en memoria a archivo
    char filename[4096];                                    // nombre del archivo destino, PATH_MAX?
    strncpy(filename, argv[1], strlen(argv[1]) - 4);        // copio el nombre menos las ultimas cuatro letras. ¿y si n 0 o negativo????
    strcat(filename, ".ppm");                               // le agrego .ppm

    file = fopen(filename, "wb");                           if (!file) return 5;

    fprintf(file, "P6\n%u %u\n255\n", wrgb, hrgb);
    fwrite(rgb, 1, wrgb * hrgb * 3, file);
    fclose(file);

    return 0;
}

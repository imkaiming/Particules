/* programme qui utilise la librairy standard du langage c
et qui génère une séquence d'échantillon se basant sur une
onde sinusoidale et le redirige  le flux vers la sortie standard */

/* calculer un cycle d'un sinus > l'enregistre dans une table >
utilise une méthode de recherche ou la wavetable synthesis technique
pour générer le signal. */

/* par défaut la sortie standard est associé à l'écran (e.g. printf("hello"))
nous pouvons rediriger la sortie standard vers un fichier en utilisant > lors
de l'appel

exemple : helloworl.c > texte.txt

donc au lieu de streamer une séquence de caractère nous allons streamer une séquence de samples
repéresentant un son.*/

#include <stdio.h>
#include <math.h>

#define SAMPLING_RATE 44100
#define NUM_SECONDS 3
#define NUM_SAMPLES (NUM_SECONDS * SAMPLING_RATE)
#define PI 3.14159265
#define FREQ 440

int main()
{
    int j;
    for (j = 0; j < NUM_SAMPLES; j++)
    {
        float sample = sin(2 * PI * FREQ * j / SAMPLING_RATE);
        printf("%f", sample);
    }

    return 0;
}

// gcc -Wall hellosine.c -o hellosine.o -lm
// ./hellosine.o > sound.txt
// cat sound.txt
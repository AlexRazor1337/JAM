#include "client.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    dyad_init();

    authorization(NULL, init_main_struct(argc, argv));

    gtk_main();
}

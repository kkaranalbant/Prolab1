#include <stdlib.h>
#include <string.h>
#include "goruntu.h"
#include "httpsreq.h"
#include "models.h"
#include "parser.h"

int main() {

    initModelArrays();
    char* url = malloc(200 * sizeof (char));
    printf("Lutfen Senaryo Dosyasinin Bulundugu Urlyi Giriniz : \n");
    scanf("%199s", url);
    char* data = makeHttpsRequest(url);
    writeToFile("/home/kaan/Downloads/json3/json/real_scenario.json", data);
    
    parseFile("/home/kaan/Downloads/json3/json/unit_types.json", true, false, false, false);
    parseFile("/home/kaan/Downloads/json3/json/heroes.json", false, false, true, false);
    parseFile("/home/kaan/Downloads/json3/json/research.json", false, true, false, false);
    parseFile("/home/kaan/Downloads/json3/json/creatures.json", false, false, false, true);
    parseFile("/home/kaan/Downloads/json3/json/real_scenario.json",false,false,false,false);

    visualize() ;

}
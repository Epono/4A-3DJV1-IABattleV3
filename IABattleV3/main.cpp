#include "Army.hpp"
#include <cstdlib>
#include <time.h>
#include <iostream>
#include "training.hpp"
#include "fstream"
#include "Extractor.hpp"
#include "Tree.hpp"


int main(void)
{
	std::stringstream ss("?C1LDOPU<C4U!ALDOPU?aDOPU<a5O!EBO!N");
	Unit* u = nullptr;
	Army* a = nullptr, * o = nullptr;
	Node* tree = TreeFactory::buildTree(ss, u, a, o);
    std::srand(time(NULL));
    std::vector<std::shared_ptr<Army> > champions;
    try {
        std::ifstream in("Army_10_100.save");
        Army army = Army::load(in);
        std::cout<<army<<std::endl;
        champions.push_back(std::shared_ptr<Army>(new Army(army)));
    } catch(...) {

    }
    std::shared_ptr<Army> army = train(10,100, 20, 10, 100, champions);
    std::cout<<*army<<std::endl;
    std::ofstream out("Army_10_100.save");
    army->save(out);
    out.flush();
    out.close();
    return 0;
}
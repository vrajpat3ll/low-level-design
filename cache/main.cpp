#include "cache.cpp"
#include "evict.cpp"
#include "storage.cpp"

int main() {
    int choice;

    std::unique_ptr<Storage<int, int>> store = std::make_unique<Storage<int, int>>();
    std::unique_ptr<LRUEvictionPolicy<int>> policy = std::make_unique<LRUEvictionPolicy<int>>();
    LRUCache<int, int> cache(std::move(store), std::move(policy), 3);

    do {
        std::cout << "Choose option\n\t[1] Put\n\t[2] Get\n\t[3] Remove\n\t[9] Quit" << std::endl;
        std::cin >> choice;

        int x, y;
        switch (choice) {
            case 1: {
                std::cout << "Enter key:";
                std::cin >> x;
                std::cout << "Enter value:";
                std::cin >> y;
                cache.put(x, y);
                break;
            }
            case 2: {
                std::cout << "Enter key:";
                std::cin >> x;
                auto got = cache.get(x);
                if (got.has_value())
                    std::cout << "Got " << got.value() << std::endl;
                else
                    std::cout << "Key does not exist!" << std::endl;
                break;
            }
            case 3: {
                std::cout << "Enter key to remove:";
                std::cin >> x;
                cache.remove(x);
                std::cout << "Removed " << x << std::endl;
                break;
            }
            default:
                break;
        }

    } while (choice != 9);

    return 0;
}

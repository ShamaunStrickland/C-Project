#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Function to generate random number between min and max
int random(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Function to clear the console screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Structure to represent a sword
struct Sword {
    string name;
    int damageIncrease;
    int rarity; // 1: Common, 2: Uncommon, 3: Rare, 4: Epic, 5: Legendary

    bool operator==(const Sword& other) const {
        return name == other.name;
    }
};

// Function to simulate a fight with a monster
bool fight(int playerAttack, int& playerHealth, int& monsterHealth) {
    monsterHealth -= playerAttack;
    int monsterAttack = random(5, 15);
    playerHealth -= monsterAttack;

    cout << "You hit the monster for " << playerAttack << " damage. Monster's health: " << monsterHealth << endl;
    cout << "Monster hits you for " << monsterAttack << " damage. Your health: " << playerHealth << endl;

    if (playerHealth <= 0) {
        cout << "You were defeated by the monster. Game over!" << endl;
        return false;
    } else if (monsterHealth <= 0) {
        cout << "You defeated the monster!" << endl;
        return true;
    }
    return true;
}

// Function to simulate the boss fight
bool bossFight(int playerHealth, int& bossHealth) {
    cout << "You encounter the boss!" << endl;
    cout << "Boss's health: " << bossHealth << endl;

    while (playerHealth > 0 && bossHealth > 0) {
        int playerAttack = random(15, 30);
        int bossAttack = random(15, 25);

        bossHealth -= playerAttack;
        playerHealth -= bossAttack;

        cout << "You hit the boss for " << playerAttack << " damage. Boss's health: " << bossHealth << endl;
        cout << "Boss attacks you for " << bossAttack << " damage. Your health: " << playerHealth << endl;

        if (playerHealth <= 0) {
            cout << "You were defeated by the boss. Game over!" << endl;
            return false;
        } else if (bossHealth <= 0) {
            cout << "Congratulations! You defeated the boss!" << endl;
            return true;
        }
    }
    return false;
}

bool findChest(bool& hasHealingPotion, vector<Sword>& playerSwords, int& playerHealth, int& monsterHealth) {
    int chance = random(1, 100);
    if (chance <= 25) { // 25% chance of finding a chest
        cout << "You found a chest!" << endl;
        int chestType = random(1, 100);
        if (chestType <= 50 && !hasHealingPotion) { // 50% chance of getting a healing potion
            cout << "You got a healing potion!" << endl;
            hasHealingPotion = true;
        } else { // 50% chance of getting a weapon or encountering a mimic
            if (random(1, 100) <= 25) { // 25% chance of encountering a mimic
                cout << "You encountered a mimic!" << endl;
                int enemyHealth = random(20, 35);
                int enemyAttack = random(5, 10);
                bool result = fight(enemyAttack, playerHealth, enemyHealth); // Fight with the mimic
                if (result) {
                    // Player defeated the mimic, so they get the chest item
                    int potionChance = random(1, 100);
                    if (potionChance <= 50 && !hasHealingPotion) { // 50% chance of getting a healing potion
                        cout << "You got a healing potion from the mimic!" << endl;
                        hasHealingPotion = true;
                    } else {
                        Sword sword;
                        int rarity = random(1, 5); // Random rarity level
                        switch (rarity) {
                            case 1:
                                sword.name = "Wooden Sword";
                                sword.damageIncrease = 5;
                                sword.rarity = 1;
                                break;
                            case 2:
                                sword.name = "Iron Sword";
                                sword.damageIncrease = 10;
                                sword.rarity = 2;
                                break;
                            case 3:
                                sword.name = "Steel Sword";
                                sword.damageIncrease = 15;
                                sword.rarity = 3;
                                break;
                            case 4:
                                sword.name = "Golden Sword";
                                sword.damageIncrease = 20;
                                sword.rarity = 4;
                                break;
                            case 5:
                                sword.name = "Legendary Sword";
                                sword.damageIncrease = 25;
                                sword.rarity = 5;
                                break;
                        }
                        playerSwords.push_back(sword);
                        cout << "You defeated the mimic and obtained the chest item: " << sword.name << "!" << endl;
                    }
                    return true;
                } else {
                    cout << "Game Over! You were defeated by the mimic." << endl;
                    return false;
                }
            } else { // 75% chance of getting a weapon
                Sword sword;
                int rarity = random(1, 5); // Random rarity level
                switch (rarity) {
                    case 1:
                        sword.name = "Wooden Sword";
                        sword.damageIncrease = 5;
                        sword.rarity = 1;
                        break;
                    case 2:
                        sword.name = "Iron Sword";
                        sword.damageIncrease = 10;
                        sword.rarity = 2;
                        break;
                    case 3:
                        sword.name = "Steel Sword";
                        sword.damageIncrease = 15;
                        sword.rarity = 3;
                        break;
                    case 4:
                        sword.name = "Golden Sword";
                        sword.damageIncrease = 20;
                        sword.rarity = 4;
                        break;
                    case 5:
                        sword.name = "Legendary Sword";
                        sword.damageIncrease = 25;
                        sword.rarity = 5;
                        break;
                }

                // Check if the player already has a sword with the same name and rarity
                auto existingSword = find_if(playerSwords.begin(), playerSwords.end(), [&](const Sword& s) {
                    return s.name == sword.name && s.rarity == sword.rarity;
                });

                if (existingSword != playerSwords.end()) {
                    // Replace the existing sword only if the new sword is of a higher rarity level
                    if (sword.rarity > existingSword->rarity) {
                        *existingSword = sword;
                        cout << "You got a " << sword.name << "!" << endl;
                    } else {
                        cout << "You found a sword, but you already have a better one." << endl;
                    }
                } else {
                    playerSwords.push_back(sword);
                    cout << "You got a " << sword.name << "!" << endl;
                }
            }
        }
    } else if (chance <= 50) { // 25% chance of encountering an enemy
        cout << "You encountered an enemy!" << endl;
        int enemyType = random(1, 4); // Random enemy type
        int enemyHealth, enemyAttack;
        switch (enemyType) {
            case 1:
                enemyHealth = random(20, 35);
                enemyAttack = random(5, 10);
                break;
            case 2:
                enemyHealth = random(15, 25);
                enemyAttack = random(8, 15);
                break;
            case 3:
                enemyHealth = random(10, 20);
                enemyAttack = random(10, 20);
                break;
            case 4:
                enemyHealth = random(5, 15);
                enemyAttack = random(15, 25);
                break;
        }
        bool result = fight(enemyAttack, playerHealth, enemyHealth); // Fight with the enemy
        if (!result) // Player was defeated by the enemy
            return false;
    } else {
        cout << "You found nothing." << endl;
    }
    return true;
}

const Sword& findRarestSword(const vector<Sword>& playerSwords) {
    if (playerSwords.empty()) {
        // Return a dummy Sword object to indicate that no sword is equipped
        static const Sword noSword = {"No Sword", 0, 0};
        return noSword;
    }

    const Sword* rarestSword = &playerSwords[0];
    for (size_t i = 1; i < playerSwords.size(); ++i) {
        if (playerSwords[i].rarity > rarestSword->rarity) {
            rarestSword = &playerSwords[i];
        }
    }
    return *rarestSword;
}

void printEquippedRarestItem(const vector<Sword>& playerSwords, bool hasHealingPotion) {
    if (playerSwords.empty()) {
        cout << "Player has no swords." << endl;
    } else {
        const Sword& rarestSword = findRarestSword(playerSwords);
        cout << "Equipped Rarest Sword: " << rarestSword.name << " (Rarity: " << rarestSword.rarity << ")" << endl;
    }

    if (hasHealingPotion) {
        cout << "Player has a healing potion." << endl;
    } else {
        cout << "Player has no potions." << endl;
    }
}

// Function to draw tunnel animation for Left turn
void drawTunnelLeft() {
    cout << R"(
---------------------------------------


--------------              -----------
             |              |
             |              |
             |              |
             |              |
             |              |

)";
    this_thread::sleep_for(chrono::milliseconds(200)); // Delay for 0.5 second

    cout << R"(
---------------------------------------


--------------              -----------
             |              |
             |              |
             |              |
             |              |

)";
    this_thread::sleep_for(chrono::milliseconds(200)); // Delay for 0.5 second

    cout << R"(
---------------------------------------


--------------              -----------
             |              |
             |              |
             |              |

)";
    this_thread::sleep_for(chrono::milliseconds(200)); // Delay for 0.5 second

    cout << R"(
---------------------------------------


--------------              -----------
             |              |
             |              |

)";
    this_thread::sleep_for(chrono::milliseconds(200)); // Delay for 0.5 second

    cout << R"(
-------------------------
                          )
                            )
    ----------
             |              |


)";
    this_thread::sleep_for(chrono::milliseconds(200)); // Delay for 0.5 second

    cout << R"(
                     -
                       -
         -                -
          -                )
           -                 )
            -
             |              |


)";
    this_thread::sleep_for(chrono::milliseconds(200)); // Delay for 0.5 second

    cout << R"(


       -             -
        -              -
          -               -
           -               -
             -              -
             -              -


)";
    this_thread::sleep_for(chrono::milliseconds(200)); // Delay for 0.5 second

    cout << R"(
                    -             -
                    -             -
                    -             -
                    -             -
                    -             -
                    -             -


)";
    this_thread::sleep_for(chrono::milliseconds(200)); // Delay for 0.5 second

    cout << R"(
---------------------------------------


--------------              -----------
             |              |
             |              |
             |              |
             |              |
             |              |

)";
    this_thread::sleep_for(chrono::milliseconds(200)); // Delay for 0.5 second

    clearScreen(); // Move clearScreen() after animation loop
}

// Function to draw tunnel animation for right turn
void drawTunnelRight() {
    cout << R"(
---------------------------------------


--------------              -----------
             |              |
             |              |
             |              |
             |              |
             |              |

)";
    this_thread::sleep_for(chrono::milliseconds(200)); // Delay for 0.5 second

    cout << R"(
---------------------------------------


--------------              -----------
             |              |
             |              |
             |              |
             |              |

)";
    this_thread::sleep_for(chrono::milliseconds(200)); // Delay for 0.5 second

    cout << R"(
 ---------------------------------------


--------------              -----------
             |              |
             |              |
             |              |
)";
    this_thread::sleep_for(chrono::milliseconds(200)); // Delay for 0.5 second

    cout << R"(
---------------------------------------


--------------              -----------
             |              |
             |              |

)";
    this_thread::sleep_for(chrono::milliseconds(200)); // Delay for 0.5 second

    cout << R"(
                  ---------------------------
                (
             (
             -              -----------
             |              |



)";
    this_thread::sleep_for(chrono::milliseconds(200)); // Delay for 0.5 second

    cout << R"(
                        -----
                     --
                  -                -
                (               -
             (                -
             -              ---


)";
    this_thread::sleep_for(chrono::milliseconds(200)); // Delay for 0.5 second

    cout << R"(
                       -             -
                     -              -
                  -                -
                -               -
             -                -
             -              -


)";
    this_thread::sleep_for(chrono::milliseconds(200)); // Delay for 0.5 second

    cout << R"(
                    -             -
                    -             -
                    -             -
                    -             -
                    -             -
                    -             -


)";
    this_thread::sleep_for(chrono::milliseconds(200)); // Delay for 0.5 second

    cout << R"(
---------------------------------------


--------------              -----------
             |              |
             |              |
             |              |
             |              |
             |              |

)";
    this_thread::sleep_for(chrono::milliseconds(200)); // Delay for 0.5 second

    clearScreen(); // Move clearScreen() after animation loop
}

int main() {
    srand(time(nullptr)); // Seed the random number generator

    cout << "Welcome to the Cave of Trials!" << endl;

    int playerHealth = 100;
    int bossHealth = 200;
    bool hasHealingPotion = false;
    vector<Sword> playerSwords;

    for (int trail = 1; trail <= 50; ++trail) {
        cout << "You completed trail " << trail << "." << endl;
        cout << "==============================" << endl;

        if (trail % 10 == 0) { // Boss encounter
            if (!bossFight(playerHealth, bossHealth)) {
                break;
            } else {
                if (trail == 50) {
                    cout << "Congratulations! You completed all trails and defeated the final boss!" << endl;
                } else {
                    string exitChoice;
                    cout << "Do you want to exit? (Y/N): ";
                    cin >> exitChoice;
                    if (exitChoice == "Y" || exitChoice == "y") {
                        cout << "Thanks for playing! Exiting the game." << endl;
                        break;
                    }
                }
            }
        } else { // Regular trail
            string directionChoice;
            bool validChoice = false;
            while (!validChoice) {
                cout << "----------------------------------------     You are at a fork in the cave. Choose left or right (l/r)"
                        "                                     \n"
                        "                                     \n"
                        "--------------              ------------     Type 'Show Inventory' to display items\n"
                        "             |              |   \n"
                        "             |              |   \n"
                        "             |              |   \n"
                        "             |              |   \n"
                        "             |              |   \n"
                        "             |              |   \n"
                        "             |              |   \n"
                        "Choice (R or L) : ";

                cin >> directionChoice;
                transform(directionChoice.begin(), directionChoice.end(), directionChoice.begin(), ::tolower); // Convert to lowercase

                if (directionChoice == "show inventory") {
                    printEquippedRarestItem(playerSwords, hasHealingPotion); // Call printEquippedRarestItem instead
                    continue; // Skip the rest of the loop and ask for direction choice again
                }

                if (directionChoice == "r" || directionChoice == "l") {
                    validChoice = true;
                } else {
                    cout << "Invalid choice. Please enter R or L for Right or Left, or 'Show Inventory' to display your items." << endl;
                }
            }

            bool result = false;
            int monsterHealth = 100; // Initialize monster health

            if (directionChoice == "l") {
                drawTunnelLeft();
                cout << "You went left." << endl;
                cout << endl;
                result = findChest(hasHealingPotion, playerSwords, playerHealth, monsterHealth);
            } else if (directionChoice == "r") {
                drawTunnelRight();
                cout << "You went right." << endl;
                cout << endl;
                result = findChest(hasHealingPotion, playerSwords, playerHealth, monsterHealth);
            }

            if (result) {
                cout << "Proceeding to the next trail..." << endl;
                this_thread::sleep_for(chrono::milliseconds(1000)); // Delay for 1 second
            } else {
                cout << "Game Over! You failed to complete the trail." << endl;
                break;
            }
        }
    }

    return 0;
}

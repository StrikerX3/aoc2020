#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>

using u64 = uint64_t;

struct Food {
    std::unordered_set<std::string> ingredients;
    std::unordered_set<std::string> allergens;
};

struct FoodCollection {
    std::vector<Food> foods;
    std::unordered_map<std::string, std::string> ingredientToAllergen;

    void computeAllergens() {
        // Allergen -> possible ingredients
        std::unordered_map<std::string, std::unordered_set<std::string>> possibleAllergenIngredients;

        // Find the possible ingredients for each unique allergen
        // - When an allergen is found for the first time, build a set with all the food's ingredients
        // - When an allergen is found subsequent times, intersect the possible allergen set
        for (auto& food : foods) {
            for (auto& allergen : food.allergens) {
                if (possibleAllergenIngredients.contains(allergen)) {
                    auto& possibleIngredients = possibleAllergenIngredients.at(allergen);
                    std::unordered_set<std::string> ingredientsToRemove;
                    for (auto& ingredient : possibleIngredients) {
                        if (!food.ingredients.contains(ingredient)) {
                            ingredientsToRemove.insert(ingredient);
                        }
                    }
                    for (auto& ingredient : ingredientsToRemove) {
                        possibleIngredients.erase(ingredient);
                    }
                }
                else {
                    possibleAllergenIngredients[allergen] = food.ingredients;
                }
            }
        }

        // At this point, some allergens still have more than one possible ingredient, but at least one of them has exactly one possible ingredient.
        // Filter out ingredients using the allergens that have exactly one possible ingredient.
        for (;;) {
            bool allOnes = true;
            for (auto& [allergen, ingredients] : possibleAllergenIngredients) {
                if (ingredients.size() == 1) {
                    // Remove the allergen from the other sets
                    for (auto& [allergen2, ingredients2] : possibleAllergenIngredients) {
                        if (allergen2 == allergen) continue; // but not from itself
                        ingredients2.erase(*ingredients.begin());
                    }
                }
                else { // size > 1
                    allOnes = false;
                }
            }
            // Stop when all sets have been reduced to one allergen each
            if (allOnes) break;
        }

        // Construct backwards dictionary (ingredient -> allergen)
        for (auto& [allergen, ingredients] : possibleAllergenIngredients) {
            for (auto& ingredient : ingredients) {
                ingredientToAllergen[ingredient] = allergen;
            }
        }
    }
};

void part1(const FoodCollection& foods) {
    // Print out the mappings
    for (auto& [ingredient, allergen] : foods.ingredientToAllergen) {
        std::cout << ingredient << " -> " << allergen << '\n';
    }

    // Count number of ingredients that are safe
    u64 safe = 0;
    for (auto& food : foods.foods) {
        for (auto& ingredient : food.ingredients) {
            if (!foods.ingredientToAllergen.contains(ingredient)) {
                safe++;
            }
        }
    }

    std::cout << "part 1: " << safe << '\n';
}

void part2(const FoodCollection& foods) {
    // Sort ingredients alphabetically by their allergen
    std::map<std::string, std::string> allergenToIngredient;
    for (auto& [ingredient, allergen] : foods.ingredientToAllergen) {
        allergenToIngredient.insert({ allergen, ingredient });
    }

    std::vector<std::string> ingredients;
    for (auto& [allergen, ingredient] : allergenToIngredient) {
        ingredients.push_back(ingredient);
    }

    std::ostringstream ss;
    std::copy(ingredients.cbegin(), ingredients.cend(), std::ostream_iterator<std::string>(ss, ","));
    auto result = ss.str();

    std::cout << "part 2: " << result << '\n';
}

auto loadInput() {
    FoodCollection foods;
    std::ifstream f{ "input.txt" };
    std::string line;
    while (std::getline(f, line)) {
        auto splitPos = line.find(" (contains");
        std::istringstream ingredients{ line.substr(0, splitPos) };
        std::istringstream allergens{ line.substr(splitPos + 11, line.size() - splitPos - 12) };

        Food food;
        std::string value;
        while (ingredients >> value) {
            food.ingredients.insert(value);
        }
        while (std::getline(allergens, value, ',')) {
            if (value[0] == ' ') value = value.substr(1);
            food.allergens.insert(value);
        }
        foods.foods.push_back(food);
    }
    foods.computeAllergens();
    return foods;
}

int main() {
    auto foods = loadInput();
    part1(foods);
    part2(foods);
    return 0;
}

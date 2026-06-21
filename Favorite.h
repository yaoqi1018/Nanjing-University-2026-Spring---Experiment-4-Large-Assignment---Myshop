#ifndef FAVORITE_H
#define FAVORITE_H

#include <string>

using namespace std;

void addFavorite(string userName);
void addFavoriteById(string userName, int id);
void deleteFavorite(string userName);
void showFavorite(string userName);
void favoriteToCart(string userName);

#endif

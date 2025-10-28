#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cstring>
using namespace std;

struct card
{
    const char* suit;
    const char* symbol;
    int points;
};

struct deck
{
    card* cards;
    int size;
};

void fill_pile(card* cards, const char* suits_arr[], const char* symbols_arr[], int points_arr[])
{
    int pos=0;
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<13; j++)
        {
            cards[pos] = {suits_arr[i], symbols_arr[j], points_arr[j]};
            pos++;
        }
    }
}

void take_cards(deck *from, deck *to, int n)
{
    srand(time(NULL));
    while (n>0)
    {
        int i = rand() % from->size;

        card taken = from->cards[i];

        card* new_arr_dealer = new card[from->size-1];

        int pos = 0;
        for (int j = 0; j<from->size; j++)
        {
            if (j != i)
            {
                new_arr_dealer[pos] = from->cards[j];
                pos++;
            }
        }

        delete[] from->cards;
        from->cards = new_arr_dealer;
        from->size --;

        card* new_arr_player = new card[to->size+1];
        for (int k = 0; k<to->size; k++)
        {
            new_arr_player[k] = to->cards[k];
        }
        new_arr_player[to->size] = taken;
        delete[] to->cards;
        to->cards = new_arr_player;
        to->size++;

        n--;
    }
}

int get_points(deck *get)
{
    int res = 0;
    int as = 0;
    bool k = false;

    for (int i = 0; i < get->size; i++)
    {
        if (strcmp(get->cards[i].symbol, "A") != 0)
        {
            res += get->cards[i].points;
        }
        else
        {
            as += 1;
        }
    }

    if (as != 0)
    {
        for (int j = as; j>0; j--)
        {
            if (res + 11 > 21)
            {
                res+=1;
            }
            else
            {
                res+=11;
                k = true;
            }
        }

        if (res > 21 && k == true)
        {
            res -= 10;
        }
    }

    return res;
}

void player_turns_write(deck *player, deck *dealer)
{
    cout << ">Player's hand:              Dealer's hand:\n";
    int i = 0;
    while (i<player->size)
    {
        if (i == 0)
        {
            cout << ">" << player->cards[i].symbol << " of " << player->cards[i].suit << "                   " << dealer->cards[i].symbol << " of " << dealer->cards[i].suit << "\n";
        }
        if (i == 1)
        {
            cout << ">" << player->cards[i].symbol << " of " << player->cards[i].suit << "                  UNKNOWN\n";
        }
        if (i != 0 && i != 1)
        {
            cout << ">" << player->cards[i].symbol << " of " << player->cards[i].suit << "\n";
        }
        i++;
    }
    cout <<"      |                           |\n";
    cout <<"      V                           V\n";
    if (dealer->cards[0].symbol == "A")
    {
        cout << ">" << get_points(player) << " points                    " << "11 points\n";
    }
    else
    {
        cout << ">" << get_points(player) << " points                      " << dealer->cards[0].points << " points\n";
    }
}

bool player_turn(deck *player, deck *dealer, deck *pile)
{
    bool player_turn_over = false;
    bool start_dealer_turn = true;

    while (player_turn_over == false)
    {
        player_turns_write(player, dealer);
        if (get_points(player) == 21)
        {
            cout << "\n>BLACK JACK!!!\n";
            player_turn_over = true;
            continue;
        }
        else
        {
            if (get_points(player) > 21)
            {
                cout << "\n>BUST!!!\n";
                cout << ">The dealer's second card was a " << (dealer->cards[1]).symbol << " of " << (dealer->cards[1]).suit << "\n";
                cin.ignore();
                start_dealer_turn = false;
                player_turn_over = true;
                continue;
            }
            else
            {
                string res;
                cout << ">Write \"HIT\" or \"STAND\"\n\n";
                cin>> res;

                while (res != "STAND" && res != "HIT")
                {
                    cout << ">Invalid input\n>Write \"HIT\" or \"STAND\"\n";
                    cin >> res;
                }

                if (res == "STAND")
                {
                    player_turn_over = true;
                    continue;
                }
                else
                {
                    cout << "\n>Picking a card from the pile...\n";
                    take_cards(pile, player, 1);
                }
            }
        }
    }
    return start_dealer_turn;
}

void dealer_turns_write(deck *player, deck *dealer)
{
    cout << ">Player's hand:              Dealer's hand:\n";
    int i = 0;
    int j = 0;

    while(i<player->size && j<dealer->size)
    {
        cout << ">" << player->cards[i].symbol << " of " << player->cards[i].suit << "                   " << dealer->cards[j].symbol << " of " << dealer->cards[j].suit << "\n";
        i++;
        j++;
    }

    while(i<player->size)
    {
        cout << ">" << player->cards[i].symbol << " of " << player->cards[i].suit << "\n";
        i++;
    }

    while(j<dealer->size)
    {
        cout << "      |                    " << dealer->cards[j].symbol << " of " << dealer->cards[j].suit << "\n";
        j++;
    }

    cout <<"      |                           |\n";
    cout <<"      V                           V\n";
        
    cout << ">" << get_points(player) << " points                    " << get_points(dealer) << " points\n";
}

int raw_points(deck *get)
{
    int sum = 0;

    for (int i = 0; i<get->size; i++)
    {
        sum += (get->cards[i]).points;
    }
    return sum;
}

void dealer_turn(deck *player, deck *dealer, deck *pile)
{
    bool dealer_turn_over = false;

    while(dealer_turn_over == false)
    {
        cin.ignore();
        dealer_turns_write(player, dealer);
        if (get_points(dealer)>17 || (get_points(dealer)==17 && get_points(dealer) == raw_points(dealer)))
        {
            dealer_turn_over = true;
        }
        else
        {
            take_cards(pile, dealer, 1);
        }
    }
}

bool is_a_num(string n)
{
    bool res = true;
    for (int i = 0; n[i] != '\0'; i++)
    {
        if (!(48<=n[i] && n[i]<=57))
        {
            res = false;
        }
    }
    return res;
}

int to_num(string n)
{
    int res = 0;

    for (int i = 0; n[i] != '\0'; i++)
    {
        res *= 10;
        res += n[i] - 48;
    }

    return res;
}

int do_bet(int n)
{
    string inp;
    cout << ">You have "<< n << " chips.\n>Write the bet value.\n";
    cin >> inp;
    while (!is_a_num(inp) || to_num(inp)>n)
    {
        cout << ">Invalid input.\n";
        cout << ">You have "<< n << " chips.\n>Write the bet value.\n";
        cin >> inp;
    }
    return to_num(inp);
}

int main()
{
    cout << " ____  _            _          _            _    \n" << "| __ )| | __ _  ___| | __     | | __ _  ___| | __ \n"<< "|  _ \\| |/ _` |/ __| |/ /  _  | |/ _` |/ __| |/ / \n" << "| |_) | | (_| | (__|   <  | |_| | (_| | (__|   < \n" << "|____/|_|\\__,_|\\___|_|\\_\\  \\___/ \\__,_|\\___|_|\\_\\\n";
    cout << "\n\n>Welcome to the Black Jack table, a program made in C++ \n>that simulates a simplified version of the game.\n>Hope you like it :)\n--------------ENTER TO START---------------\n";
    cin.ignore();

    const char* suits_arr[4] = {"Hearts", "Spades", "Diamonds", "Clubs"};
    const char* symbols_arr[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Q", "J", "K", "A"};
    int points_arr[13] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 1};

    cout << ">How many chips do you want to start with??\n";
    int chips = 0;
    string inp;
    cin >> inp;

    while(chips <= 0){
        while (!is_a_num(inp) || to_num(inp) <= 0)
        {
            cout << ">Invalid input.\n>How many chips do you want to start with???\n";
            cin >> inp;
        }

        chips = to_num(inp);
    }


    while (chips != 0)
    {
        card* pile_arr = new card[52];
        card* player_hand_arr = new card[0];
        card* dealer_hand_arr = new card[0];

        deck pile = {pile_arr, 52};
        deck player_hand = {player_hand_arr, 0};
        deck dealer_hand = {dealer_hand_arr, 0};

        int bet = do_bet(chips);

        cin.ignore();
        cout << ">shuffling cards...\n";
        cout << "\n>PLAYER'S TURN\n";
        fill_pile(pile.cards, suits_arr, symbols_arr, points_arr);
        take_cards(&pile, &player_hand, 2);
        take_cards(&pile, &dealer_hand, 2);

        bool start_dealer_turn = player_turn(&player_hand, &dealer_hand, &pile);


        if (start_dealer_turn)
        {
            cout << "\n>DEALER'S TURN\n";
            dealer_turn(&player_hand, &dealer_hand, &pile);
        }

        if (((get_points(&player_hand) > get_points(&dealer_hand)) && get_points(&player_hand)<=21) || get_points(&dealer_hand) > 21)
        {
            cout << "\n>PLAYER WINS";
            if (get_points(&player_hand) == 21)
            {
                cout << "\n>You won " << bet+(bet/2) << " chips.";
                chips += bet+(bet/2);
                cout << "\n>Now you have " << chips << " chips.";
            }
            else
            {
                cout << "\n>You won " << bet << " chips.";
                chips += bet;
                cout << "\n>Now you have " << chips << " chips.";
            }
        }
        if (get_points(&player_hand) == get_points(&dealer_hand))
        {
            cout << "\n>TIE";
            cout << "\n>You got your bet back.";
            cout << "\n>Now you have " << chips << " chips.";
        }
        if (((get_points(&player_hand) < get_points(&dealer_hand)) && get_points(&dealer_hand)<=21) || (get_points(&player_hand) > 21))
        {
            cout << "\n>DEALER WON";
            chips -= bet;
            cout << "\n>You lost " << bet << " chips.";
            cout << "\n>Now you have " << chips << " chips.";
        }

        delete[] pile.cards;
        delete[] player_hand.cards;
        delete[] dealer_hand.cards;

        if (chips > 0)
        {
            cout << "\n--------------ENTER TO START---------------\n";
            cin.ignore();
        }
        else
        {
            cout << "\n-------------BETTER LUCK NEXT TIME ;)---------------\n";
            cin.ignore();
        }
    }

    return 0;
}
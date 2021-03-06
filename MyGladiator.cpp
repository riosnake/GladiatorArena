#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <math.h>
using namespace std;
random_device rng;
// THE GLADIATOR CLASS YOU'LL BE MODIFYING.

class Gladiator {
protected:

    // These are for restoring our gladiator to fighting condition.
    int originalAtk;
    int originalDef;
    int originalSpd;

public:

    // VARIABLES YOU HAVE ACCESS TO:
    string name;
    int maxHP;  // Max HP of your gladiator. it should ALWAYS be 150
    int HP;  // current HP. 

    // NOTE: ATK + DEF + SPD == 150
    int atk;
    int def;
    int spd;

    int wins; // keeps track of how many wins your gladiator has


    // constructor for the gladiator, you get to choose the initial stats down in main
    explicit Gladiator(string name, int atk, int def, int spd) {
        this->name = name;
        this->atk = atk;
        this->def = def;
        this->spd = spd;
        wins = 0;
        maxHP = 150;
        HP = 150;
        this->originalSpd = spd;
        this->originalAtk = atk;
        this->originalDef = def;
    }

    // IMPORTANT: if you add any extra variables to your gladiator (like arrows, mana, etc.) make sure you set them back to the original value
    // they should have within this function!
    virtual void restoreCondition() {
        HP = maxHP;
        atk = originalAtk;
        def = originalDef;
        spd = originalSpd;
    }


    // simple check on if the gladiator is alive
    bool isAlive() {
        return HP > 0;
    }

    // simple check on if the gladiator is dead
    bool isDead() {
        return HP <= 0;
    }

    // how a gladiator takes damage.
    // damage taken is proportionally reduced by defense (So, for example, a player with 150 defense will only take 6% of the original damage.)
    virtual void takeDamage(int amount) {

        int damage = amount * (1 - (def / 160.0));
        if (damage < 0) {
            damage = 0;
        }
        cout << name << "'s defense reduces the damage to " << damage << endl;
        HP -= damage;

    }

    // speed determines who goes first, as well as how many times a gladiator can attack on their turn.
    // Essentially, this multiplier is proportional to the enemies speed.
    // if you have about twice as much speed as the other gladiator, you'll attack two times.
    // if you have 3x as much speed, attack 3 times, etc.
    // this speed multiplier caps at x4 normally, but if the opponents speed is 0, it's x6.
    // DO NOT MODIFY WITHOUT RATH'S PERMISSION
    int calculateSpeedMultiplier(int other_spd) {

        // make sure we're not dividing by 0
        // dont let your speed fall to 0. is bad.
        if (other_spd == 0) {
            return 6;
        }
        if (spd / other_spd <= 0) {
            return 1;
        }
        else {
            int multiplier = round(spd / (other_spd + 10.0));
            if (multiplier > 4) {
                return 4;
            }
            else {
                return multiplier;
            }
        }
    }

    // how a gladiator attacks another gladiator
    // damage can be calculated as:
    // attack * spdMultiplier - (attack * (enemyDefense / 160))
    virtual void attack(Gladiator& enemy) {
        for (int i = 0; i < calculateSpeedMultiplier(enemy.spd); i++) {
            int damageDealt = atk;
            cout << name << " deals " << damageDealt << " damage to " << enemy.name << endl;
            enemy.takeDamage(damageDealt);
        }
    }

    virtual void choose_action(Gladiator& enemy) {
        /*
        THIS IS WHERE YOU CODE WHAT ACTION YOUR GLADIATOR SHOULD TAKE ON THEIR TURN
        Your gladiator will only perform ONE action on their turn. (No calling attack(enemy) 20 times)
        Actions are things such as buffing their stats, applying debuffs to enemies, performing special moves, etc.

        The only thing to keep in mind though is that YOU have to code those actions.

        So, you can add cool mechanics or special attacks all you want, as long as you can code it.
        If Rath decides a mechanic is too unbalanced or unfair, he will ask you to change/adjust it. Otherwise, assume everything is allowed.

        you can access any variables of another players gladiator or your own gladiator, and edit them accordingly

        IF YOU WANT TO DEAL DAMAGE TO A GLADIATOR: you should use enemy.takeDamage(amount of damage).
        This will take defense of your enemy into account, instead of just flat damage.

        When deciding what action to choose, you have to CODE WHAT THE COMPUTER SHOULD CHOOSE.
        This can be in the form of random chance, or prioritizing certain actions depending on current hp, enemy hp, etc.

        As an example, if I created a gladiator with an attack buff mechanic, I might choose to only choose that action when
        my attack is below a certain number through if-else statements, or i might leave it to random chance when i apply the buff.


        */
        attack(enemy);
    }

    // WHAT YOUR GLADIATOR WILL SAY WHEN IT BEGINS FIGHTING
    virtual void warcry() {

        // edit this with a CAMP APPROPRIATE message for when your gladiator begins fighting
        cout << "ARGH! I didnt bother changing this default battle cry!" << endl;
    }

    // WHAT YOUR GLADIATOR WILL SAY WHEN IT LOSES
    virtual void defeatCry() {

        // edit this with a CAMP APPROPRIATE message for when your gladiator loses
        cout << "I lost, and i didnt bother changing this default defeat cry" << endl;
    }

    // WHAT YOUR GLADIATOR WILL SAY WHEN IT WINS
    virtual void victoryCry() {

        // edit this with a CAMP APPROPRIATE message for when your gladiator wins
        cout << "I won, and i didnt bother changing this default victory cry" << endl;
    }

    // PUT ANY CUSTOM FUNCTIONS YOU WANT TO WRITE BELOW:


    // DO NOT MODIFY ANYTHING BELOW
};

// Dont worry about this, it just sets up a dummy to fight you
class TrainingDummy : public Gladiator {
public:
    TrainingDummy(string name, int atk, int def, int spd) : Gladiator(name, atk, def, spd) {
        cout << "" << endl;
    }

    void choose_action(Gladiator& enemy) override {
        cout << "Im a training dummy! hit me!" << endl;
        attack(enemy);
    }

    void warcry() override {
        cout << "Im a training dummy! please hit me to your heart's content." << endl;
    }

    void defeatCry() override {
        cout << "Good Job!" << endl;
    }

    // WHAT YOUR GLADIATOR WILL SAY WHEN IT WINS
    void victoryCry() override {
        cout << "...Looks like you need to fix that, i should lose..." << endl;
    }
};

// What allows your gladiator to fight the training dummy.
class Arena {
public:
    random_device rng;


    void round(Gladiator& first, Gladiator& second) {
        cout << first.name << "'s turn!" << endl;
        this_thread::sleep_for(chrono::seconds(2));
        first.choose_action(second);
        this_thread::sleep_for(chrono::seconds(2));

        if (second.isDead()) {
            return;
        }
        cout << second.name << "'s turn!" << endl;
        this_thread::sleep_for(chrono::seconds(2));
        second.choose_action(first);

        this_thread::sleep_for(chrono::seconds(2));
    }

    bool determine_first(Gladiator& player1, Gladiator& player2) {
        // Check who goes first
        if (player1.spd > player2.spd) {
            return true;
        }
        else if (player1.spd < player2.spd) {
            return false;
        }
        else {
            //random coin toss if tie!!!
            if (rng() % 2 == 0) {
                return true;
            }
            else {
                return false;
            }
        }
    }
    // how two gladiators fight in the game.
    void fight(Gladiator& player1, Gladiator& player2) {
        this_thread::sleep_for(chrono::seconds(4));
        player1.warcry();
        this_thread::sleep_for(chrono::seconds(4));
        player2.warcry();
        this_thread::sleep_for(chrono::seconds(2));
        bool player1_faster;
        int turn_limit = 10;
        int turn = 1;
        // main game loop
        while (player1.isAlive() && player2.isAlive() && turn <= turn_limit) {
            cout << "Turn " << turn << "\n";
            player1_faster = determine_first(player1, player2);

            // faster player fights first.
            if (player1_faster) {
                round(player1, player2);
            }
            else {
                round(player2, player1);
            }

            cout << "------------------------------------------------------------------------------------------------------\n";
            cout << player1.name << "'s current HP: " << player1.HP << endl;
            cout << player2.name << "'s current HP: " << player2.HP << endl;
            cout << "------------------------------------------------------------------------------------------------------\n" << endl;
            this_thread::sleep_for(chrono::seconds(4));

            turn += 1;
        }

        // honestly this probably wont happen but in case both players die
        // each player is awarded a win
        if (player1.isDead()) {
            cout << player2.name << " WINS!!!!!!" << endl;
            player1.defeatCry();
            player2.victoryCry();
            player2.wins += 1;
        }
        if (player2.isDead()) {
            cout << player1.name << " WINS!!!!!!" << endl;
            player2.defeatCry();
            player1.victoryCry();
            player1.wins += 1;
        }
        // if time ran out, gladiator with the most hp left wins.
        if (turn > turn_limit) {
            if (player1.HP > player2.HP) {
                cout << player1.name << " wins by health." << endl;
                player2.defeatCry();
                player1.victoryCry();
                player1.wins += 1;
            }
            else if (player2.HP > player1.HP) {
                cout << player2.name << " wins by health." << endl;
                player1.defeatCry();
                player2.victoryCry();
                player2.wins += 1;
            }
            else {
                cout << "Both players tie. Boring! Both lose!" << endl;
                player1.defeatCry();
                player2.defeatCry();
            }
        }

        // restore fighters HP for the next fight
        player1.restoreCondition();
        player2.restoreCondition();

    }


};
int main() {

    // An example of how the program works.
    Arena arena;
    TrainingDummy dummy("Training Dummy", 50, 50, 50);
    
    // IMPORTANT: This is your gladiator. Set their starting stats and name here.
    Gladiator your_gladiator("Buff dude", 100, 50, 50);
    arena.fight(your_gladiator, dummy);

}
#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    if (score1 > score2)
    {
        printf("Player 1 Wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 Wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    int x = strlen(word);
    int sum = 0;
    // دورة فور هنا تمر على كل حرف حتى تصل لطول الكلمة
    for (int i = 0; i < x; i++)
    {

        // لا بد من تصفير السكور مع كل دورة
        // لأن في حالة إذا كان العنصر هو "رمز" ستتكرر دورة فور على آخر قيمة لها و تضيفها على المجموع

        int score = 0;

        // هنا نرى إذا كان الحرف الإنجليزي كبير أم صغير
        // فإذا كان صغيرا نقلل من فهرس الرقم عدد (97) لأن هذا هو فهرس أول رقم إنجليزي صغير في الأسكي

        if (isupper(word[i]) == 0)
        {

            // هنا نتأكد من أن فهرس الحرف هو بين 96 إلى 123 لكي نستبعد الرموز من الأحرف

            if (word[i] > 96 && word[i] < 123)
            {
                score = POINTS[word[i] - 97];
            }
        }
        else
        {

            // هنا في حالة إذا كان الحرف صغيرا نضعه بين 64 و 91

            if (word[i] > 64 && word[i] < 91)
            {
                score = POINTS[word[i] - 65];
            }
        }

        sum = score + sum;
    }
    return sum;
}

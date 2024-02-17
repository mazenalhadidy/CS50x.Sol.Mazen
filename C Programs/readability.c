//برنامج يحسب عدد الحروف و عدد الكلمات و عدد الجمل
//ليضعها في معادلة كولمان ليو ليحدد أي النصوص مناسبة لأي عمر حسب طولها

#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

int countletters(string a);
int countwords(string b);
int countsent(string c);

int main(void)
{
    string text = get_string("Text: ");
    int noLetters = countletters(text);
    int noWords = countwords(text);
    int noSent = countsent(text);

    float L = (float) noLetters * 100 / (float) noWords;
    float S = (float) noSent * 100 / (float) noWords;

    //معادلة كولمان ليو (Coleman Liau)
    float index = 0.0588 * L - 0.296 * S - 15.8;
    int Grade = (int)round(index);

    if (Grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (Grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", Grade);
    }

}

//-----------------------------------------------------------------
int countletters(string a)
{
    int x = strlen(a);
    int counter = 0;
    for (int i = 0; i < x; i++)
    {
        //يمر على كل حرف في السترينغ و يفحصه حتى يصل لآخر حرف في طول الستنرينغ
        // a >> z, A >> Z
        if ((a[i] > 64 && a[i] < 91) || (a[i] > 96 && a[i] < 123))
        {
            counter++;
        }
    }
    return counter;
}

//-------------------------------------------------------------
int countwords(string b)
{
    int x = strlen(b);
    int counter = 0;

    for (int i = 0; i < x; i++)
    {
        //عدد الكلمات هو عدد مرات تكرار " " المسافة.
        //بافتراض أنه لن يبدأ بمسافة و لن يكتب مسافتين متكررتين و أنه يوجد دائما كلمة في بداية الجمل.
        if (b[i] == 32)
        {
            counter++;
        }
    }
    counter = counter + 1;
    return counter;
}

//---------------------------------------------------------------


int countsent(string c)
{
    int x = strlen(c);
    int counter = 0;

    for (int i = 0; i < x; i++)
    {
        //عدد الجمل هو عدد مرات تكرار "." أو "؟" أو "!" هذه الرموز.
        //تحتاج لتعديل مستقبلا حتى لا يخلط بين النقطة التي هي نهاية الجملة و انقطة للتعبير عن اختصار مثلا
        if (c[i] == 33 || c[i] == 46 || c[i] == 63)
        {
            counter++;
        }
    }
    return counter;
}
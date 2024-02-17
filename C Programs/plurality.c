#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    //دورة للمرور على كل الأسماء المدرجة لمعرفة إن كانت مثل الاسم المكتوب كمرشحين
    for (int i = 0; i < candidate_count; i++)
    {
        //يقارن الاسم، فإن كان كاسم المرشح المحفوظ يزيد في عدد أصواته
        //ثم يرجع "ترو" للدالة
        if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // المتغير "كي" لتحديد أكبر عدد أصوات
    int k = candidates[0].votes;

    //عداد لمعرفة عدد المنتخبين ذوي الأصوات الكبرى المتساوية
    int c = 0;

    for (int i = 0; i < candidate_count - 1; i++)
    {
        //كلما كانت "كي" ثايتة في المنتخب التالي يزيد "سي" فيعبر عن زيادة عدد المنتخبين ذوي الأصوات المساوية الكبرى
        if (k == candidates[i + 1].votes)

        {
            c++;
        }

        //إن وجد عدد أصوات أكبر يغير قيمة ال"كي" ويرجع عداد ال"سي" إلى صفر
        if (candidates[i + 1].votes > k)

        {
            c = 0;
            k = candidates[i + 1].votes;
        }
    }

    string fayez[c + 1];

    //عداد للمصطفة لوضع أسماء المرشحين فيها
    int d = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        //كلما كان عدد الأصوات مساويا للعدد الأكبر للأصوات يضع الاسم في المصطفة
        if (candidates[i].votes == k)

        {
            fayez[d] = candidates[i].name;
            d++;
        }
    }

    //دورة لطباعة كل الأسماء الموجودة في المصطفة
    for (int i = 0; i <= c; i++)
    {
        printf("%s\n", fayez[i]);
    }
    return;
}

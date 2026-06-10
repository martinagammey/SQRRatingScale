#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT 1024

/* Simple helper to clamp a value between min and max */
double clamp(double v, double lo, double hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* Map percentage (0-100) to 0.0-1.0 */
double pct_to_unit(double p) {
    return clamp(p / 100.0, 0.0, 1.0);
}

/* Convert combined score (0.0-1.0) to integer 1-5 */
int to_rating(double s) {
    /* Ensure s in [0,1], then map to 1..5 */
    s = clamp(s, 0.0, 1.0);
    /* Use rounding to nearest integer bucket */
    int rating = (int)(s * 4.0 + 0.5) + 1;
    if (rating < 1) rating = 1;
    if (rating > 5) rating = 5;
    return rating;
}

int main(void) {
    char query[MAX_INPUT];
    char url[MAX_INPUT];
    char buf[MAX_INPUT];
    double needs_pct = 0.0, eeat_pct = 0.0, spam_pct = 0.0;

    printf("Search query: ");
    if (!fgets(query, sizeof(query), stdin)) return 0;
    query[strcspn(query, "\r\n")] = '\0';

    printf("URL: ");
    if (!fgets(url, sizeof(url), stdin)) return 0;
    url[strcspn(url, "\r\n")] = '\0';

    /* For automation, we would analyze the page; here we prompt for simple indicators. */
    printf("\nEnter a number 0-100 for each of the following indicators.\n");

    printf("Needs met. Is the page related to the query, and does it satisfy what the user is looking for? (0 = not at all, 100 = fully): ");
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    needs_pct = atof(buf);

    printf("EEAT. Are the authors or is the company behind the page trustworthy, have a high profile, or actually know what they're talking about? (0 = very low, 100 = very high): ");
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    eeat_pct = atof(buf);

    printf("Spam likelihood. If the page is inaccessible or locked behind a paywall, insert a value of at least 75. (0 = not spammy, 100 = very spammy): ");
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    spam_pct = atof(buf);

    /* Normalize inputs */
    needs_pct = clamp(needs_pct, 0.0, 100.0);
    eeat_pct  = clamp(eeat_pct,  0.0, 100.0);
    spam_pct  = clamp(spam_pct,  0.0, 100.0);

    /* Convert to unit range */
    double needs = pct_to_unit(needs_pct);
    double eeat  = pct_to_unit(eeat_pct);
    double spam  = pct_to_unit(spam_pct);

    /* Weighted combination:
       - The higher the needs met and EEAT ratings, the higher the score.
       - Spam is the inverse. The higher the spam, the lower the score.
       Weights chosen so positive max can reach near 1.0, spam can reduce substantially.
       Adjust weights to taste as needed or desired.
    */
    const double w_needs = 0.45;
    const double w_eeat  = 0.45;
    const double w_spam  = 0.30; /* subtractive weight */

    double positive = w_needs * needs + w_eeat * eeat; /* in [0, w_needs+w_eeat] */
    double negative = w_spam * spam; /* in [0, w_spam] */

    /* raw score = positive - negative, then normalized to 0..1 */
    double raw = positive - negative;

    /* Maximum possible positive is w_needs+w_eeat, minimum negative is 0.
       Minimum raw could be 0 - w_spam = -w_spam.
       Normalize by shifting by +w_spam and dividing by (w_needs+w_eeat + w_spam)
    */
    double min_raw = -w_spam;
    double max_raw = w_needs + w_eeat;
    double norm = (raw - min_raw) / (max_raw - min_raw); /* now in 0..1 */
    norm = clamp(norm, 0.0, 1.0);

    int rating = to_rating(norm);

    printf("\n--- Evaluation ---\n");
    printf("Query: %s\n", query);
    printf("URL: %s\n", url);
    printf("\nComponent inputs (0-100):\n");
    printf("  Needs met: %.1f\n", needs_pct);
    printf("  EEAT:      %.1f\n", eeat_pct);
    printf("  Spam:      %.1f\n", spam_pct);

    printf("\nComputed values:\n");
    printf("  Combined normalized score: %.3f (0.0 low .. 1.0 high)\n", norm);
    printf("  Final rating (1-5): %d\n", rating);

    printf("\nExplanation:\n");
    printf("  - Higher Needs met & EEAT increase the score; higher Spam decreases it.\n");
    printf("  - You can tune weights in the source (w_needs, w_eeat, w_spam) for different behavior.\n");
    printf("  - The rating scale as follows:\n");
    printf("  - 1- Lowest.\n");
    printf("  - 2- Low.\n");
    printf("  - 3- Medium.\n");
    printf("  - 4- High.\n");
    printf("  - 5- Highest.\n");

    return 0;
}
# SQRRatingScale
A simple code, written in C utilizing VSCodium, that automatically generates a code based on user input. The rating scale is based on the official Google Search Quality Rating guidelines as a basic overview of rating URLs for search queries. You can apply these rules to other search engines as well, if you do not use Google. The program calculates a 1–5 quality rating for search results based on three user-provided metrics: Needs Met, EEAT (Experience, Expertise, Authoritativeness, Trustworthiness), and Spam Likelihood. Attatched to this document are some examples of search queries you can enter when the code asks for user input.

## Overview
This tool implements a weighted scoring algorithm to evaluate search result quality. It takes a search query and URL as context, then asks the user to input percentage scores (0–100) for three key indicators. The program normalizes these values, applies a weighted formula (where spam acts as a penalty), and outputs a final normalized score and integer rating.

## How it works

The evaluation logic follows these steps:
1. Input: Accepts a query, URL, and three percentage scores.
2. Normalization: Converts 0–100 inputs to a 0.0–1.0 range.
3. Weighted Calculation:
* Positive Factors: Needs Met (45%) and EEAT (45%).
* Negative Factor: Spam (30% penalty).
* Formula: Score = (Needs + EEAT) - Spam (normalized to 0–1).
4. Rating Conversion: Maps the final 0.0–1.0 score to a 1–5 integer rating.

# Usage
## compilation
Requires a standard C compiler. It may vary depending on which OS your computer has.

``` bash
gcc -o sqrscale sqrscale.c
```

*(If you copy any part of this code, you can replace the "sqrscale.c" with whatever filename you choose. Same thing for the "sqrscale" part. This is simply what I've named this.)*

## Running
Execute the program with the following prompts

```bash

./sqrscale
```
### Example session
```text
Search query: best mechanical keyboards
URL: https://example.com/keyboard-review

Enter a number 0-100 for each of the following indicators.
Needs met...: 90
EEAT...: 85
Spam likelihood...: 10

--- Evaluation ---
Query: best mechanical keyboards
URL: https://example.com/keyboard-review

Component inputs (0-100):
  Needs met: 90.0
  EEAT:      85.0
  Spam:      10.0

Computed values:
  Combined normalized score: 0.892 (0.0 low .. 1.0 high)
  Final rating (1-5): 4

Explanation:
  - Higher Needs met & EEAT increase the score; higher Spam decreases it.
  - You can tune weights in the source (w_needs, w_eeat, w_spam) for different behavior.
  - The rating scale as follows:
  - 1- Lowest.
  - 2- Low.
  - 3- Medium.
  - 4- High.
  - 5- Highest.
```
## Rating Scale
* 1- Lowest
* 2- Low
* 3- Medium
* 4- High
* 5- Highest

## Configuration
You can adjust the influence of each factor by modifying the weight constants in the main function:

```c
const double w_needs = 0.45; // Weight for Needs Met
const double w_eeat  = 0.45; // Weight for EEAT
const double w_spam  = 0.30; // Weight for Spam (subtracted)
```
**Note:** Changing weights will alter the maximum possible positive score and the severity of the spam penalty. Ensure the sum of weights remains logical for your desired scoring distribution.

## Input Guidelines
**Needs Met (0-100):** Does the page meet user intent?
* 0-20 - Fails to Meet (FailsM)
* 21-40 - Slightly Meets (SM)
* 41-60 - Moderately Meets (MM)
* 61-80- Highly Meets (HM)
* 81-100-  Fully Meets (FullyM)
  
**E-E-A-T (0-100):** Does the source demonstrate a substantial level of **E**xpertise, **E**xperience, **A**uthoritativeness, and/or **T**rust? Does the creator of the main content have a positive or generally verified reputation?
* 0-20 - Anonymous, untrustworthy,unverified
* 21-40 - Somewhat untrustworthy, likely a creator of the mc with a negative reputation
* 41-60 - Slightly trustworthy, likely unverified or invalid
* 61-80 - Trustworthy, may or may not ve verified
* 81-100 - Highly trustworthy, positive reputation

**Spam Likelihood (0–100)**: How likely is this to be spam or inaccessible?
* 0-20 - Clean, accessible content. Adblockers or extensions that disable cookies/trackers not required.
* 21-40 - Slightly spammy, may have ads that could overlap the content (should have an option to close it out).
* 41-60 - Moderately spammy, most likely will have unskippable ads that overlap the content. May have malicious cookies.
* 61-80 - Highly spammy, may be full of ads that will unintentially redirect to actual spam sites. Likely to infect device with malware.
* 81-100 - Definite spam and/or clickbait, with malicious intent. Highly likely to cause device to be infected with malware. This scoring should automatically rate the page quality to "Lowest".

## Dependencies
Standard C Library (stdio.h, string.h, stdlib.h)

No external dependencies required.

## Additional comments
Feel free to clone this code and modify it to suit your needs.
Feel free to contribute to this project or point out any problems or issues with this code.

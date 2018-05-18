# DynamicPairwiseAlignmentExtension

Implementation of a dynamic edit distance matrix which dynamically keeps track of the edit distance of two strings, allowing the following operations with linear update time:
- inserting a character to the beginning of a string
- inserting a character to the end of a string
- deleting a character from the beginning of a string
- deleting a character from the end of a string

The implementation is based on Lemma 4 of the paper "Dynamic Edit Distance Table under a General Weighted Cost Function" by Heikki Hyyrö, Kazuyuki Narisawa, and Shunsuke Inenaga (https://dl.acm.org/citation.cfm?id=2822788).

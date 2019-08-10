#!/usr/bin/python

# Given a word, return a string that has a "y" for every letter that is correct
# and in the correct position and a "n" for every letter that is correct but in the
# wrong position. 
def check_word(target, guess):
	# TODO this assumes they are the same length
	pairs = zip(target, guess)
	answer = []
	keep_list = []
	# Get all the letters that are correct and in the correct place
	for idx, p in enumerate(pairs):
		if p[0] == p[1]:
			answer.append("y")
		else:
			keep_list.append(p)

	# keep_list now contains all the pairs that don't have exact matches
	# Turn the list of tuples back into a couple of lists
	target_remain, guess_remain = zip(*keep_list)
	target_remain = list(target_remain)
	guess_remain = list(guess_remain)

	for g_letter in guess_remain:
		for idx, t_letter in enumerate(target_remain):
			if g_letter == t_letter:
				answer.append('n')
				# Ok to delete stuff out of a list because we're not continuing to operate on it
				target_remain.pop(idx)
				break

	return answer

# a = "boy"
# b = "kob"
# print a
# print b
# print check_word(a,b)


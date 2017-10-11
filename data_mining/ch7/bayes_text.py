import os

import math


class BayesText:
    # ['classA', 'classB']
    categories = []
    # word count dict
    # vocabulary = {'hello' : 1, 'word' : 4, 'foo' : 3, 'bar' : 5}
    vocabulary = {}
    # every word in category probabilities
    # prob = {'classA' : {'hello' : 0.001, 'word' : 0.023},
    #         'classB' : {'foo' : 0.002, 'bar' : 0.033} }
    prob = {}
    # every category total word counts
    # totals = {'classA' : 521, 'classB': 428}
    totals = {}

    def __init__(self, train_dir, stop_word_list_file):
        """
        This class implements a native Bayes approach to text classification
        :param train_dir: Each subdirectory of train_dir is titled with the name of the classification category.
        :param stop_word_list_file: A list of words (one word per line) will be removed before any counting takes place.
        """
        stop_words = {}
        with open(stop_word_list_file) as f:
            for line in f:
                stop_words[(line.strip())] = 1
            f.close()
        self.categories = [category for category in os.listdir(train_dir)
                           if os.path.isdir(train_dir + category)]
        print('Counting ... ')
        for category in self.categories:
            print('\t', category)
            (self.prob[category], self.totals[category]) = self.word_count(train_dir, category, stop_words)

        # Eliminate any word in vocabulary than doesn't occur at least 3 times
        to_delete = []
        for word in self.vocabulary:
            if self.vocabulary[word] < 3:
                # mark word for deletion
                # cannot delete now because you cannot delete from a list you are iterating over
                to_delete.append(word)
        # Now delete
        for word in to_delete:
            del self.vocabulary[word]
        # Now compute probabilities
        vocabulary_len = len(self.vocabulary)
        print('Computing probabilities :')
        for category in self.categories:
            print('\t', category)
            denominator = self.totals[category] + vocabulary_len
            for word in self.vocabulary:
                if word in self.prob[category]:
                    count = self.prob[category][word]
                else:
                    count = 1
                self.prob[category][word] = (float(count + 1) / denominator)
        print('Done training\n\n')

    def word_count(self, train_dir, category, stop_words):
        current_dir = train_dir + category
        files = os.listdir(current_dir)
        counts = {}
        total = 0
        for file in files:
            # codecs.open(current_dir + '/' + file, 'r', 'iso8859-1')
            with open(current_dir + '/' + file) as f:
                for line in f:
                    tokens = line.split()
                    for token in tokens:
                        # Get rid of punctuation and lowercase token
                        token = token.strip('\'".,?:-').lower()
                        if token != '' and token not in stop_words:
                            self.vocabulary.setdefault(token, 0)
                            self.vocabulary[token] += 1
                            counts.setdefault(token, 0)
                            counts[token] += 1
                            total += 1
                f.close()
        return (counts, total)

    def classify(self, filename):
        results = {}
        with open(filename) as f:
            for line in f:
                tokens = line.split()
                for token in tokens:
                    token = token.strip('\'".,?-').lower()
                    if token in self.vocabulary:
                        for category in self.categories:
                            if self.prob[category][token] != 0:
                                results[category] += math.log(self.prob[category][token])
            f.close()
        results = list(results.items())
        results.sort(key=lambda tuple:tuple[1], reverse=True)
        return results[0][0]


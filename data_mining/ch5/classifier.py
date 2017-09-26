import math


class Classifier:
    prior = {}
    condition = {}
    means = {}
    ssd = {}

    def __init__(self, bucket_prefix, test_bucket_number, data_format, delimiter=','):
        total_instance_count = 0

        # every class counts
        # eg. class_counts = {'classA' : 100, 'classB' : 100}
        class_counts = {}
        # counts used for attributes that are not numeric
        # 离散类型的属性 数量统计
        # eg. discrete_attr_counts = {
        # 'classA' : {'attr1' : { 'yes' : 50, 'no': 50},
        #             'attr2' : { 'low' : 40, 'mid' : 10, 'high' : 50 } },
        # 'classB' : {'attr1' : { 'yes' : 80, 'no': 20},
        #             'attr2' : { 'low' : 25, 'mid' : 32, 'high' : 43 } }
        # }
        discrete_attr_counts = {}
        # totals used for attributes that are numeric
        # 数值类型的属性　所有值的和, 用来计算平均值
        # eg. numeric_attr_totals = {
        # 'classA' : {'num1' : 123, 'num2' : 456 },
        # 'classB' : {'num1' : 789, 'num2' : 222 }
        # }
        numeric_attr_totals = {}
        # dict for numeric attributes original data
        # 数值类型的属性　所有数据的列表，用来计算样本标准差
        # eg. numeric_attr_values = {
        # 'classA' : {'num1' : [3, 4, 1, ...], 'num2' : [2, 1, 5, ...]},
        # 'classB' : {'num1' : [2, 1, 0, ...], 'num2' : [12, 0, 34, ...]}
        # }
        numeric_attr_values = {}

        self.format = data_format.strip().split(delimiter)
        # 10 折交叉验证的 10 个桶的数据
        for i in range(1, 11):
            if i != test_bucket_number:
                filename = "%s-%02i" % (bucket_prefix, i)
                with open(filename, 'r') as f:
                    lines = f.readlines()
                    f.close()
                for line in lines:
                    fields = line.strip().split(delimiter)
                    category = ''
                    discrete_attrs = []
                    numeric_attrs = []
                    ignore = []
                    for i in range(len(fields)):
                        if self.format[i] == 'num':
                            numeric_attrs.append(float(fields[i]))
                        elif self.format[i] == 'attr':
                            discrete_attrs.append(fields[i])
                        elif self.format[i] == 'class':
                            category = fields[i]
                        else:
                            ignore.append(fields[i])
                    # Now process this instance
                    total_instance_count += 1
                    class_counts.setdefault(category, 0)
                    discrete_attr_counts.setdefault(category, {})
                    numeric_attr_totals.setdefault(category, {})
                    numeric_attr_values.setdefault(category, {})
                    class_counts[category] += 1
                    for attr_idx, val in enumerate(discrete_attrs):
                        discrete_attr_counts[category].setdefault(attr_idx, {})
                        discrete_attr_counts[category][attr_idx].setdefault(val, 0)
                        discrete_attr_counts[category][attr_idx][val] += 1
                    for attr_idx, val in enumerate(numeric_attrs):
                        numeric_attr_totals[category].setdefault(attr_idx, 0.0)
                        numeric_attr_values[category].setdefault(attr_idx, [])
                        numeric_attr_totals[category][attr_idx] += val
                        numeric_attr_values[category][attr_idx].append(val)
        # Now compute probabilities
        for (category, count) in class_counts.items():
            self.prior[category] = count / total_instance_count
        for (category, attr_counts) in discrete_attr_counts.items():
            self.condition.setdefault(category, {})
            for (attr, attr_count) in attr_counts.items():
                self.condition[category].setdefault(attr, {})
                for (val, count) in attr_count.items():
                    self.condition[category][attr][val] = count / class_counts[category]
        # Now compute mean and sample standard deviation
        for (category, attr_totals) in numeric_attr_totals.items():
            self.means.setdefault(category, {})
            for (attr, totals) in attr_totals.items():
                self.means[category][attr] = totals / class_counts[category]
        for (category, attr_values) in numeric_attr_values:
            self.ssd.setdefault(category, {})
            for (attr, values) in attr_values.items():
                sum_diff2 = 0
                mean = self.means[category][attr]
                count = class_counts[category]
                for value in values:
                    sum_diff2 += (value - mean) ** 2
                self.ssd[category][attr] = math.sqrt(sum_diff2 / (count - 1))

    def pdf(self, mean, ssd, x):
        e_part = math.pow(math.e, -(x - mean) ** 2 / (2 * ssd ** 2))
        return (1.0 / (math.sqrt(2 * math.pi) * ssd)) * e_part

    def classify(self, discrete_vectors, numeric_vectors):
        results = []
        for (category, prior) in self.prior.items():
            prob = prior
            for (attr, val) in discrete_vectors:
                if val not in self.condition[category][attr]:
                    prob = 0
                else:
                    prob *= self.condition[category][attr][val]
            for (attr, val) in numeric_vectors:
                mean = self.means[category][attr]
                ssd = self.ssd[category][attr]
                prob *= self.pdf(mean, ssd, val)
            results.append((prob, category))

        return max(results)[1]

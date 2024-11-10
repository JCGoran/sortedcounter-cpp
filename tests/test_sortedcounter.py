from copy import copy

import numpy as np
import pytest

from sortedcounter import SortedCounter

rng = np.random.default_rng(4092)

data_size = 1_000
data = rng.integers(-10, 10, data_size)

def test_basics():
    tree = SortedCounter(data)
    assert len(tree) == len(data)
    assert tree.maximum() == max(data)
    assert tree.minimum() == min(data)

def test_addition():
    tree = SortedCounter(data)
    tree_copy = copy(tree)
    size = 10
    tree.add(data[0], size)
    assert len(tree_copy) + size == len(tree)

def test_delitem():
    tree = SortedCounter([1, 2, 3, 3])
    tree.remove(3)
    assert len(tree) == 3
    tree.remove(3)
    assert len(tree) == 2
    with pytest.raises(ValueError):
        tree.remove(3)
    tree.remove(2)
    tree.remove(1)
    assert len(tree) == 0

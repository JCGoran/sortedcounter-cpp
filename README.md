# SortedCounterCPP

## What's this?

A Python package that's like the built-in [`collections.Counter`](https://docs.python.org/3/library/collections.html#collections.Counter), but sorted.
Access to min/max elements is guaranteed to have $O(\log n)$ time complexity.

## Compatibility

CPython only for now.

The API currently supports only elements of C++ type `double` (equivalent to Python's `float`).

## License

See [LICENSE](./LICENSE).

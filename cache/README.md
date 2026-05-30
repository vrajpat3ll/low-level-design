# Background

A cache is a set of keys and values that is used to store the values of a previous computation.

## Functional Requirements

1. Needs to be able to store keys and their correspondiing values, as well as performing operations like read, delete, update on those keys and values.
2. Needs to be able to evict certain elements of the cache when space is limited.

## Design Philosophy

There are many different ways that a key-value (or KV) store can be implemented, and there are many ways that an eviction policy can be implemented.

Hence, we want to asbtract them away into interfaces, so that a cache can be created with multiple different type of classes.

## Cache Interface

- Cache(Interface<Storage> storage, Interface<EvictionPolicy> evictionPolicy, uint capacity)
- get(Key key)
- put(Key key, Value value)
- delete(Key key, Value value)

Cache is an interface because depending on the type of storage we use, we may be able to support more operations like getting ordered set of values by key, or by time added. We can also have an update operation but for now let's just use (delete and put) for now.

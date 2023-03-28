#pragma once

class OurStack {
public:
    OurStack();
    ~OurStack();

    void push(int value);
    int pop();
    int peek() const;

    int size() const;
    bool isEmpty() const;
private:
  void setCapacity(int capacity);

  int* elems;
  int logicalSize;
  int allocatedSize;
};

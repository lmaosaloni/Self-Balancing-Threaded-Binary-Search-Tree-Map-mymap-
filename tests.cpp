#include <gtest/gtest.h>
#include <map>
#include <string>
#include <iostream>
#include <sstream>

#include "mymap.h"
#include "myrandom.h"

TEST(mymap, one) {
  // testing:
  //         - default constructor
  
  // int, int
  mymap<int, int> m1;
  EXPECT_EQ(m1.Size(), 0);
  
  // string, int
  mymap<string, int> m2;
  EXPECT_EQ(m2.Size(), 0);
  
  // char, double
  mymap<char, double> m3;
  EXPECT_EQ(m3.Size(), 0);
  
  // string, string
  mymap<string, string> m4;
  EXPECT_EQ(m4.Size(), 0);
}


TEST(mymap, two) {
  //
  // testing: - Size()
  //          - put()
  //          - contains()
  //          - get()
  //          - operator[]
  //
  
  // (int, int)
  mymap<int, int> m1;
  int arr1[] = {2, 1, 3};
  // adding elements and checking size
  for (int i = 0; i < 3; ++i)
      m1.put(arr1[i],arr1[i]);
  EXPECT_EQ(m1.Size(), 3);
  
  // checking if the values are inserted or not
  // contains
  EXPECT_EQ(m1.contains(2), true);
  EXPECT_EQ(m1.contains(1), true);
  EXPECT_EQ(m1.contains(3), true);
  EXPECT_EQ(m1.contains(4), false);
  // get
  EXPECT_EQ(m1.get(2), 2);
  EXPECT_EQ(m1.get(1), 1);
  EXPECT_EQ(m1.get(3), 3);
  //checking if get adds an absent key
  EXPECT_EQ(m1.get(4), 0);
  EXPECT_EQ(m1.contains(4), false);
  // operator[]
  EXPECT_EQ(m1[1], 1);
  EXPECT_EQ(m1[2], 2);
  EXPECT_EQ(m1[3], 3);
  EXPECT_EQ(m1[4], 0);         // testing the 'add' feature of []
  EXPECT_EQ(m1.Size(), 4);
  m1.put(4, 4);                // checking if the value updates or not with put()
  EXPECT_EQ(m1.get(4), 4);
  
  // toString
  string correct = "";
  for (int i = 1; i < 5; ++i) {
    correct += "key: ";
    correct += to_string(i);
    correct += " value: ";
    correct += to_string(i);
    correct += '\n';
  }
  EXPECT_EQ(m1.toString(), correct);
}

/*
TEST (mymap, checkThreads1) {
  int arr1[] = {4, 2, 6, 1, 3, 5, 7};
  int arr1R[] = {4, 6, 5, 7};
  int arr1L[] = {4, 2, 1, 3};
  int order1[] = {1, 2, 3, 4, 5, 6, 7};
  
  mymap<int, int> m1;
  m1.put(4,4);
  m1.put(2,2);
  m1.put(7,7);
  m1.put(1,1);
  m1.put(3,3);
  m1.put(5,5);
  m1.put(8,8);
  m1.put(6,6);
  
  m1.checkThreads();
  cout << endl;

  mymap<int, string> m2;
  string temp;
  for (int i = 0; i < 4; ++i) {
    temp = m2[ arr1R[i] ];
  }
  EXPECT_EQ(m2.contains(5), true);
  m2.checkThreads();
  cout << endl << m2.toString() << endl;
}
*/
/*
TEST (mymap, checkThreads2) {
  //
  // new order
  //
  int arr1[] = {4, 2, 7, 1, 3, 5, 8, 6};
  int arr1R[] = {4, 7, 5, 8, 6};
  int arr1L[] = {4, 2, 3, 1};
  int order1[] = {1, 2, 3, 4, 5, 6, 7, 8};
  
  mymap<int, int> m1;
  for (int i = 0; i < 8; ++i) {
    m1.put( arr1[i], arr1[i] );
  }
  
  
  // for each loop
  int i = 0;
  for (auto key:m1) {
    cout << key << " ";
    EXPECT_EQ(order1[i], key);
    i++;
  }
}
*/
/*
TEST (mymap, begin) {
  mymap<int, int> m1;
  m1.put(4,4);
  m1.put(2,2);
  m1.put(5,5);
  m1.put(1,1);
  m1.put(3,3);
  m1.put(6,6);
  m1.put(7,7);
  
  EXPECT_EQ(m1.bottomLeft(), "Key: 1 Value: 1");
  // iterator<int,int> it = m1.begin();
  // EXPECT_EQ(*it, 1);
  
} */
/*
TEST (mymap, three) {
  //
  // testing for each loop
  //
  
  mymap<int, int> m1;
  m1.put(4,4);
  m1.put(2,2);
  m1.put(5,5);
  m1.put(1,1);
  m1.put(3,3);
  m1.put(6,6);
  m1.put(7,7);
  
  int order[] = {1,2,3,4,5,6,7};
  int i = 0;
  for (auto key:m1) {
    EXPECT_EQ(order[i++], key);
  }
  auto iter = m1.begin();
  while (iter != m1.end()) {
    cout << *iter << " ";
    ++iter;
  }
  cout << endl;
  
} */

TEST (mymap, four) {
  // testing - clear()
  //         - checkBalance()
  
  mymap<int, int> m1;
  m1.put(4,4);
  m1.put(2,2);
  m1.put(6,6);
  m1.put(1,1);
  m1.put(3,3);
  m1.put(5,5);
  m1.put(7,7);
  m1.put(8,8);
  // cout << m1.checkBalance() << endl;
  
  EXPECT_EQ(m1.Size(), 8);
  m1.clear();
  EXPECT_EQ(m1.Size(), 0);
  
  mymap<int, int> m2;
  m2.put(4,4);
  m2.put(2,2);
  m2.put(7,7);
  m2.put(1,1);
  m2.put(3,3);
  // cout << m2.checkBalance() << endl;
  // should be (pre-Order): 1, 2, 4, 5, 3
  // cout << m2.toString() << endl;
  // should be (in-Order): 1, 2, 3, 4, 5
}

TEST (mymap, five) {
  // testing - copy constructor
  //         - operator=
  mymap<int, int> m1;
  m1.put(4,4);
  m1.put(2,2);
  m1.put(7,7);
  m1.put(1,1);
  m1.put(3,3);
  m1.put(5,5);
  m1.put(8,8);
  
  mymap<int, int> m1Copy(m1);
  cout << m1.checkBalance() << endl;
  EXPECT_EQ(m1.Size(), m1Copy.Size());
  EXPECT_EQ(m1.toString(), m1Copy.toString());
  
  mymap<int, int> m2;
  m2 = m1;
  EXPECT_EQ(m1.Size(), m2.Size());
  EXPECT_EQ(m1.toString(), m2.toString());
  // cout << m2.checkBalance() << endl;
  
}
/*
TEST (mymap, six) {
  // testing - toVector
  mymap<int, int> m1;
  m1.put(4,4);
  m1.put(2,2);
  m1.put(7,7);
  m1.put(1,1);
  m1.put(3,3);
  m1.put(5,5);
  m1.put(8,8);
  m1.put(6,6);
  
  vector< pair<int,int> > tree = m1.toVector();
  
} */

TEST (mymap, seven) {
  // testing - vector constructor
  //         - balancing
  vector< pair<int,int> > V1;
  for (int i = 1; i < 9; ++i) {
    pair<int,int> p;
    p.first = i;
    p.second = i;
    V1.push_back(p);
  }
  
  mymap<int, int> m1(V1);
  EXPECT_EQ(m1.Size(), 8);
  // cout << m1.toString() << endl;
  
  // balancing
  mymap<int, int> m2;
  for (int i = 1; i < 9; ++i) {
    m2.put(i,i);
  }
  
  cout << m2.toString() << endl;
  
}
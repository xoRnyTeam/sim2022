#ifndef INIT_UNIT
#define INIT_UNIT

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

#endif //! INIT_UNIT
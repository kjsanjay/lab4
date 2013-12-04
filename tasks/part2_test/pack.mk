PROGS_part2_test_OBJS := part2_test.o
PROGS_part2_test_OBJS := $(PROGS_part2_test_OBJS:%=$(TDIR)/part2_test/%)
ALL_OBJS += $(PROGS_DAGGER_OBJS)

$(TDIR)/bin/part2_test : $(TSTART) $(PROGS_part2_test_OBJS) $(TLIBC)


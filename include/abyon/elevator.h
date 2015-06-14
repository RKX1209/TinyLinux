/*
 * include/abyon/elevator.h
 * Copyright (C) <2015>  <@RKX1209>
 */
#ifndef _ABYON_ELEVATOR_H
#define _ABYON_ELEVATOR_H

struct elevator_ops{
};
struct elevator_type{
};
struct elevator_queue{
  struct elevator_ops *ops;
  void *elevator_data;
  struct elevator_type *elevator_type;
};

typedef struct elevator_queue elevator_t;
#define ELEVATOR_NO_MERGE	0
#define ELEVATOR_FRONT_MERGE	1
#define ELEVATOR_BACK_MERGE	2

/*
 * Insertion selection
 */
#define ELEVATOR_INSERT_FRONT	1
#define ELEVATOR_INSERT_BACK	2
#define ELEVATOR_INSERT_SORT	3

#endif

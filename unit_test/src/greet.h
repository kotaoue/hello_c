#ifndef GREET_H
#define GREET_H

/* 現在時刻に応じた挨拶文を返す */
const char *get_greeting(void);

/* 指定した時間（0〜23）に応じた挨拶文を返す */
const char *get_greeting_by_hour(int hour);

#endif /* GREET_H */

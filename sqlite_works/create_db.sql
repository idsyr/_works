create table content_st (
       id integer primary key autoincrement,
       file_name text not null unique,
       send_st text not null default 'no'
);

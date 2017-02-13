CREATE TABLE public.user
(
    email text NOT NULL,
    created timestamp without time zone NOT NULL DEFAULT now(),
    profile json NOT NULL default '{}'::json,
    PRIMARY KEY (email)
)

CREATE TABLE public.habits
(
    habit text,
    CONSTRAINT habits_pkey PRIMARY KEY (habit)
)

CREATE TABLE public.habit
(
    completed timestamp without time zone NOT NULL DEFAULT now(),
    user_email text NOT NULL,
    habit text NOT NULL,
    CONSTRAINT habit_pkey PRIMARY KEY (user_email, habit, completed),
    CONSTRAINT habit_user_email_fkey FOREIGN KEY (user_email)
        REFERENCES public."user" (email) MATCH SIMPLE
        ON UPDATE NO ACTION ON DELETE NO ACTION,
    CONSTRAINT habit_habit_fkey FOREIGN KEY (habit)
        REFERENCES public."habits" (habit) MATCH SIMPLE
        ON UPDATE NO ACTION ON DELETE NO ACTION
)

INSERT INTO public.habits (habit) VALUES ('dont murder'), ('on time'), ('workout'), ('brush twice'), ('sleep by 12am'), ('no sweets');
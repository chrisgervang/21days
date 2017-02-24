import * as mocha from 'mocha';
import {assert} from 'chai';

import {makeHistory, isDoneForDay} from '../../factory';
import {History} from '../../commands';

const history: History[] = [ 
    // { habit: 'brush twice', completed: "2017-02-14T08:59:19.284Z" },
    // { habit: 'brush twice', completed: "2017-02-19T02:47:01.545Z" },
    // { habit: 'brush twice', completed: "2017-02-22T06:08:38.478Z" },
    // { habit: 'brush twice', completed: "2017-02-24T07:40:40.280Z" },
    // { habit: 'dont murder', completed: "2017-02-14T08:58:14.034Z" },
    // { habit: 'dont murder', completed: "2017-02-22T06:08:48.703Z" },
    // { habit: 'dont murder', completed: "2017-02-23T00:00:00.000Z" },
    // { habit: 'dont murder', completed: "2017-02-24T07:40:32.980Z" },
    // { habit: 'no sweets', completed: "2017-02-24T07:40:56.597Z" },
    // { habit: 'on time', completed: "2017-02-22T06:08:55.953Z" },
    // { habit: 'on time', completed: "2017-02-24T07:40:46.672Z" },
    // { habit: 'sleep by 12am', completed: "2017-02-22T06:09:11.454Z" },
    // { habit: 'workout', completed: "2017-02-20T18:40:32.912Z" },
    // { habit: 'workout', completed: "2017-02-22T18:02:58.624Z" },
    { habit: 'workout', completed: "2017-02-24T07:59:00.0Z" },
    { habit: 'workout', completed: "2017-02-26T23:59:00.0Z" },
    { habit: 'workout', completed: "2017-02-27T00:01:00.0Z" },
    { habit: 'workout', completed: "2017-02-15T6:00:00.0Z" },


]

const refenceDate = "2017-02-25T12:00:00.0Z";

describe('Habit tests', () => {
    it('isDoneForDay empty LA', () => {
        assert.isFalse(isDoneForDay([], refenceDate, "America/Los_Angeles", "workout"));
    });

    it('isDoneForDay 2-25 4:00 PST, yes insert', () => {
        assert.isFalse(isDoneForDay(history, refenceDate, "America/Los_Angeles", "workout"));
    });

    it('isDoneForDay 2-24 4:00 PST, yes insert, 2-24 7:59 UTC', () => {
        assert.isFalse(isDoneForDay(history, "2017-02-24T12:00:00.0Z", "America/Los_Angeles", "workout"));
    });

    it('isDoneForDay 2-23 11:58 PST, no insert, 2-24 7:59 UTC', () => {
        assert.isTrue(isDoneForDay(history, "2017-02-24T07:58:00.0Z", "America/Los_Angeles", "workout"));
    });

    it('isDoneForDay 2-24 00:00 PST, yes insert, 2-24 7:59 UTC', () => {
        assert.isFalse(isDoneForDay(history, "2017-02-24T08:00:00.0Z", "America/Los_Angeles", "workout"));
    });

    it('isDoneForDay 2-26 16:00 PST, no insert, 2-26 23:59 UTC', () => {
        assert.isTrue(isDoneForDay(history, "2017-02-27T00:00:00.0Z", "America/Los_Angeles", "workout"));
    });

    // 1st habit was inserted at 10pm 2-14 PST, and then inserted again at 1am 2-15 PST
    it('isDoneForDay 2-15 1:00 PST, yes insert, 2-15 06:00 UTC', () => {
        assert.isFalse(isDoneForDay(history, "2017-02-15T09:00:00.0Z", "America/Los_Angeles", "workout"));
    });
})


const float sinc[] = 
{
250.0000,
253.2724,
256.5442,
259.8150,
263.0840,
266.3508,
269.6148,
272.8754,
276.1321,
279.3843,
282.6315,
285.8732,
289.1086,
292.3374,
295.5589,
298.7726,
301.9779,
305.1744,
308.3613,
311.5383,
314.7048,
317.8601,
321.0038,
324.1354,
327.2542,
330.3599,
333.4517,
336.5293,
339.5920,
342.6394,
345.6709,
348.6860,
351.6842,
354.6649,
357.6278,
360.5722,
363.4976,
366.4036,
369.2897,
372.1553,
375.0000,
377.8233,
380.6246,
383.4036,
386.1598,
388.8926,
391.6016,
394.2863,
396.9463,
399.5812,
402.1904,
404.7735,
407.3301,
409.8598,
412.3620,
414.8365,
417.2827,
419.7002,
422.0886,
424.4476,
426.7767,
429.0755,
431.3436,
433.5806,
435.7862,
437.9600,
440.1015,
442.2105,
444.2865,
446.3292,
448.3383,
450.3135,
452.2542,
454.1604,
456.0315,
457.8674,
459.6676,
461.4320,
463.1600,
464.8516,
466.5064,
468.1240,
469.7043,
471.2469,
472.7516,
474.2182,
475.6463,
477.0358,
478.3864,
479.6978,
480.9699,
482.2024,
483.3951,
484.5478,
485.6604,
486.7325,
487.7641,
488.7550,
489.7049,
490.6138,
491.4815,
492.3077,
493.0925,
493.8356,
494.5369,
495.1963,
495.8137,
496.3890,
496.9221,
497.4128,
497.8612,
498.2671,
498.6305,
498.9512,
499.2293,
499.4647,
499.6574,
499.8073,
499.9143,
499.9786,
500.0000,
499.9786,
499.9143,
499.8073,
499.6574,
499.4647,
499.2293,
498.9512,
498.6305,
498.2671,
497.8612,
497.4128,
496.9221,
496.3890,
495.8137,
495.1963,
494.5369,
493.8356,
493.0925,
492.3077,
491.4815,
490.6138,
489.7049,
488.7550,
487.7641,
486.7325,
485.6604,
484.5478,
483.3951,
482.2024,
480.9699,
479.6978,
478.3864,
477.0358,
475.6463,
474.2182,
472.7516,
471.2469,
469.7043,
468.1240,
466.5064,
464.8516,
463.1600,
461.4320,
459.6676,
457.8674,
456.0315,
454.1604,
452.2542,
450.3135,
448.3383,
446.3292,
444.2865,
442.2105,
440.1015,
437.9600,
435.7862,
433.5806,
431.3436,
429.0755,
426.7767,
424.4476,
422.0886,
419.7002,
417.2827,
414.8365,
412.3620,
409.8598,
407.3301,
404.7735,
402.1904,
399.5812,
396.9463,
394.2863,
391.6016,
388.8926,
386.1598,
383.4036,
380.6246,
377.8233,
375.0000,
372.1553,
369.2897,
366.4036,
363.4976,
360.5722,
357.6278,
354.6649,
351.6842,
348.6860,
345.6709,
342.6394,
339.5920,
336.5293,
333.4517,
330.3599,
327.2542,
324.1354,
321.0038,
317.8601,
314.7048,
311.5383,
308.3613,
305.1744,
301.9779,
298.7726,
295.5589,
292.3374,
289.1086,
285.8732,
282.6315,
279.3843,
276.1321,
272.8754,
269.6148,
266.3508,
263.0840,
259.8150,
256.5442,
253.2724,
250.0000,
246.7276,
243.4558,
240.1850,
236.9160,
233.6492,
230.3852,
227.1246,
223.8679,
220.6157,
217.3685,
214.1268,
210.8914,
207.6626,
204.4411,
201.2274,
198.0221,
194.8256,
191.6387,
188.4617,
185.2952,
182.1399,
178.9962,
175.8646,
172.7458,
169.6401,
166.5483,
163.4707,
160.4080,
157.3606,
154.3291,
151.3140,
148.3158,
145.3351,
142.3722,
139.4278,
136.5024,
133.5964,
130.7103,
127.8447,
125.0000,
122.1767,
119.3754,
116.5964,
113.8402,
111.1074,
108.3984,
105.7137,
103.0537,
100.4188,
97.8096,
95.2265,
92.6699,
90.1402,
87.6380,
85.1635,
82.7173,
80.2998,
77.9114,
75.5524,
73.2233,
70.9245,
68.6564,
66.4194,
64.2138,
62.0400,
59.8985,
57.7895,
55.7135,
53.6708,
51.6617,
49.6865,
47.7458,
45.8396,
43.9685,
42.1326,
40.3324,
38.5680,
36.8400,
35.1484,
33.4936,
31.8760,
30.2957,
28.7531,
27.2484,
25.7818,
24.3537,
22.9642,
21.6136,
20.3022,
19.0301,
17.7976,
16.6049,
15.4522,
14.3396,
13.2675,
12.2359,
11.2450,
10.2951,
9.3862,
8.5185,
7.6923,
6.9075,
6.1644,
5.4631,
4.8037,
4.1863,
3.6110,
3.0779,
2.5872,
2.1388,
1.7329,
1.3695,
1.0488,
0.7707,
0.5353,
0.3426,
0.1927,
0.0857,
0.0214,
0.0000,
0.0214,
0.0857,
0.1927,
0.3426,
0.5353,
0.7707,
1.0488,
1.3695,
1.7329,
2.1388,
2.5872,
3.0779,
3.6110,
4.1863,
4.8037,
5.4631,
6.1644,
6.9075,
7.6923,
8.5185,
9.3862,
10.2951,
11.2450,
12.2359,
13.2675,
14.3396,
15.4522,
16.6049,
17.7976,
19.0301,
20.3022,
21.6136,
22.9642,
24.3537,
25.7818,
27.2484,
28.7531,
30.2957,
31.8760,
33.4936,
35.1484,
36.8400,
38.5680,
40.3324,
42.1326,
43.9685,
45.8396,
47.7458,
49.6865,
51.6617,
53.6708,
55.7135,
57.7895,
59.8985,
62.0400,
64.2138,
66.4194,
68.6564,
70.9245,
73.2233,
75.5524,
77.9114,
80.2998,
82.7173,
85.1635,
87.6380,
90.1402,
92.6699,
95.2265,
97.8096,
100.4188,
103.0537,
105.7137,
108.3984,
111.1074,
113.8402,
116.5964,
119.3754,
122.1767,
125.0000,
127.8447,
130.7103,
133.5964,
136.5024,
139.4278,
142.3722,
145.3351,
148.3158,
151.3140,
154.3291,
157.3606,
160.4080,
163.4707,
166.5483,
169.6401,
172.7458,
175.8646,
178.9962,
182.1399,
185.2952,
188.4617,
191.6387,
194.8256,
198.0221,
201.2274,
204.4411,
207.6626,
210.8914,
214.1268,
217.3685,
220.6157,
223.8679,
227.1246,
230.3852,
233.6492,
236.9160,
240.1850,
243.4558,
246.7276
};





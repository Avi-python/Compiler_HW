
## Issues

- TODO: 當 parser 遇到這種 a % b，因為我們的 parser 不支援 %，目前我是會回傳一個 Error token，不知道要怎麼調整 Bison Grammar 讓 Grammar 可以跳過 expression 的 parsing 然後找到 ; 繼續 parsing 下去
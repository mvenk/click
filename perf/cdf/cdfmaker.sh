click cdf.click >levels.out
sed '1d' levels.out >levels.bak
mv levels.bak levels.out
python radix0_distr.py levels.out > cdf.csv
R --no-save cdf.csv ExactMatches < cdf_generic.R


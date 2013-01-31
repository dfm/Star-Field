Cover Letter
============

Dear Dr. Gallagher,

Thank you for providing the referee's report on our manuscript AJ-11369
entitled "Probabilistic Catalogs for Crowded Stellar Fields". The
referee provided thoughtful commentary and constructive criticism of our
work, which was very valuable.

We have modified the manuscript significantly to address the concerns
of the referee. We list the changes below.




Regards,
Brewer, Foreman-Mackey, Hogg


Itemized Responses
==================

"Abstract seems to be overselling the case in multiple instances; for example, "introduce" may be too strong given Stetson's work on DAOPHOT"

- We have changed "introduce" to "present". Related approaches definitely exist.

"SExtractor "significantly underestimates" and "incorrect inferences"? What do "significant" and "incorrect" mean here - something of practical importance that astronomers should be scared of? The results presented in the paper didn't convince me."

- We were referring to the fact that SExtractor couldn't find all the stars.
This would be of practical importance in some instances but not others, it depends on whether finding all the stars or quantifying the faint stars is important.
The abstract has (**NOT YET**) been clarified to be more specific and realistic about the differences between the methods.


"The analysis of test cases is very superficial and the reader is left wondering about the actual performance. For example, what is the difference in photometric ans astrometric precision between the new method and SExtractor on an *object-by-object* basis? Surely at the bright end objects must be identical! What is completeness and contamination as a function of brightness? I return to this point below - something *must be wrong* with Fig. 6 and corresponding discussion. The bottom line is that currently Sections 5 and 6 are insufficient to assess the performance of this method, both in absolute sense and relative to SExtractor's performance. The unbelievable claims about SExtractor are not substantiated."

- The idea of comparing precision on an object-by-object basis is a very good one. (**I'm working on it**).




Original Referee Report Email Appears Below
===========================================

2013-1-7

Dear Dr. Brewer,

Your manuscript,

Manuscript number: AJ-11369
Title: Probabilistic Catalogs for Crowded Stellar Fields
Authors: Brewer et al.

was sent for review to an expert referee who has returned a report. The referee appreciates the value of your contribution but has expressed some significant concerns and has made recommendations for improving the manuscript. These suggestions appear reasonable and are included at the end of this message. I urge you to take the referee's comments fully into account in preparing a revised version of your paper and hope that you will find these suggestions to be helpful in improving your interesting manuscript.

To submit a revised manuscript, click on the link below:
<http://aj.msubmit.net/cgi-bin/main.plex?el=A5FC5IFj4A1huM4I3A9sLRwP2UxKZqbO4olaeZTYAZ>

Please also outline in a cover letter the changes you have made in response to the referee's comments and submit your response as a plain text (ASCII) file, citing each referee's comment immediately followed by your response.

If you have any questions or concerns, please do not hesitate to contact us.

Regards,
Jay
Dr. John (Jay) S. Gallagher, III
Editor-in-Chief, The Astronomical Journal
W. W. Morgan Professor of Astronomy, University of Wisconsin-Madison
Email: aj@astro.wisc.edu
Tel: +1 608 265 6005
Fax: +1 608 890 2599
__________

Reviewer's Comments:

The paper "Probabilistic Catalogs for Crowded Stellar Fields" by Brewer, Foreman-Mackey and Hogg proposes to model astronomical images using a full probabilistic framework based on many catalogs instead of a single catalog as done traditionally. While this theoretically superior approach is becoming feasible in practice, and thus this paper is both relevant and timely, I have two major sets of objections and a number of smaller ones which should be addressed before publication. I want to emphasize that I do like this paper and find its topic to be appropriate for publication in AJ - it is its execution that has not (yet) met the high standards maintained by AJ.

Major objections:

1) "Philosophical"

Authors attempt to address two very different issues simultaneously, and this results in unclear exposition. First, they argue for a full probabilistic framework based on many catalogs instead of a single catalog as done traditionally. While theoretically justified, it is not obvious how important the differences with respect to standard method would be in practice. It is likely that these differences are negligible in the case of uncrowded field (N=100 case). Second, the problem of crowded fields is coupled in this discussion although crowded fields can be easily addressed with a single catalog approach (indeed DAOPHOT does it so; I was very surprised that the fact that DAOPHOT implements the psf mixture model of eq. 10 is not mentioned at all). While it is probably true that the crowded field case will present the largest differences between the single and multi-catalog approach, the exposition could and should be decoupled and more to the point. For example, a sentence suc
h as
"Here we model an image as a sum of an unknown number of point-spread functions (psf), where the spatially invariant psf is described by a parametrized analytic model." in Introduction would be very helpful.

This early presentation confusion is compounded by the discussion of parametric luminosity function determination. Although it appears that its main feature is the use of the increasingly common method of Bayesian hierarchical modeling, this is not explicitly stated. If so, then exactly the same procedure could be easily performed when using a single catalog approach - the existing discussion blurs the main points of this paper.

 2) "Technical"

The analysis of test cases is very superficial and the reader is left wondering about the actual performance. For example, what is the difference in photometric ans astrometric precision between the new method and SExtractor on an *object-by-object* basis? Surely at the bright end objects must be identical! What is completeness and contamination as a function of brightness? I return to this point below - something *must be wrong* with Fig. 6 and corresponding discussion. The bottom line is that currently Sections 5 and 6 are insufficient to assess the performance of this method, both in absolute sense and relative to SExtractor's performance. The unbelievable claims about SExtractor are not substantiated.

 Specific points:

- Abstract seems to be overselling the case in multiple instances; for example, "introduce" may be too strong given Stetson's work on DAOPHOT; SExtractor "significantly underestimates" and "incorrect inferences"? What do "significant" and "incorrect" mean here - something of practical importance that astronomers should be scared of? The results presented in the paper didn't convince me.

- p. 2: PHOTO should be photo

- p. 2, first paragraph: Can't you simplify this long discussion by just stating that catalogs are lists of best-fit model parameters?

- "very lossy", "destroy information" etc. - this makes it sound like the end of the world. Either be quantitative or add appropriate caveats; (overselling this method may be counterproductive).

- p. 2, third paragraph: I am *very* confused by the statement that "Instead of estimating..." First, there is nothing more than "estimating" that we can do whatever the method. Second, and more importantly, standard methods (as in SExtractor, photo, and most other codes) *do* estimate errors for flux, positions, and many other parameters, and these error estimates *are* propagated (in most papers) to subsequent analysis. What is *not* probabilistic about giving flux+-error?

- general comment on Introduction: typically, one knows what exactly the paper is about after reading it - not the case here.

- section 3.3: it is a major omission not to give credit to Stetson and DAOPHOT here!

- minor question: after eq. 11: why is the image not *integrated* over the pixel?

- minor comment: after eq. 13: I don't quite understand the purpose of eta

- minor comment: eqs. 14 and 15: don't you want the tolerance to be +-0.1*(xmax-xmin)? That is, what if xmin=0?

- Section 5: words like BIC and Bayesian model comparison are never mentioned; looking at the left panel in Fig. 5 I wonder if high-N models should be penalized for having more parameters (or if perhaps you already included this effect); I also wonder if the scatter in h1 and other global parameters is much larger than for the case when all the fluxes are perfectly known and only LF is determined.

- Section 5: given that you discussed most basic features of Bayesian methodology, it would make sense to explain what is "the degree of compression" and why is it important (especially when a number such as exp(3000) is floated around).

- fig. 4: pretty but useless! What is the reader supposed to do - stare at it and count all the points? It would be better to use images such as Fig. 1 and 2, and show the mean image for all catalogs, and then a few residual images (perhaps normalized by statistical errors).

- fig. 5: what are the stretch and scale in the lower left panel? How do residuals compare to statistical fluctuations?

- Section 6: Fig. 6 is very misleading (it implies that SExtractor, which was so many times successfully tested, is lousy). A much more informative plot (I tried it in practice in a similar context) would be to plot cumulative counts vs. log(flux) (bright to faint) for your method and for SExtractor on the same panel. Additional useful plot would be to match sources from the two lists within some small radius and compare their photometry and astrometry with true values. It wouldn't be surprising if SExtractor has biases in crowded field case, and such biases would be *a really strong* argument in favor of your method!

- Section 6: running SExtractor down to 0.5sigma and computing LF with all these sources is pretty silly (as the sample will have a lot of faint junk; also large errors and Malmquist bias will be important). I find the clustering of alpha1 ~ alpha2 ~ 1 strongly suspect. Also, even for a clean sample with vanishing errors, there are many pitfalls when fitting power-law LFs. I strongly suggest the authors take a look at Clauset, Shalizi, and Newman (2009). Power-Law Distributions in Empirical Data. SIAM Review 51, 661-703.

- Discussion: second paragraph on p. 15: I don't think your model is limited for what you wanted to do - that is, to assess your method from a statistical point of view. What you listed are limitations for applying your procedure to real data.

# Introduction #

**Automatic Feature Recognition** could take many forms.  It would always recognize the "feature" without user input, either before or during model processing.

# Details #

Initial thoughts of features to be recognized, how to recognize them, and what to do once recognized:
  * Drilled hole
    * A cylindrical hole in the model, usually aligned in Z.
    * Detect by looking at surfaces.  They should be symmetric about a common axis. Extremely small distortions.
    * Action: Drill canned cycle(s) with automatically detected diameter and depth. For 5-axis: automatically detect angles A,B,C.
  * Slot
    * A special case of pocket.  Narrow with round ends, _possible_ to mill with a single pass.
    * Detect by looking for parallel (or offset) faces, "capped" by (and tangent to) half-cylindrical faces.
    * Action: Attempt to use cutter the same size or slightly smaller.  Will **not** actually mill with single pass in normal circumstances.
  * Pocket
    * A local indent in a face
    * Detect like Andreas did?
    * Separate pases, don't "drop in" and "pop out" for rough or finish.
  * Radius between faces, at least one of which is not vertical
    * Detect by looking for a surface with edges that are arcs with a common radius, where arcs are in a vertical plane.
    * Action: Select ballnose bit with "best" radius and trace surface(s).
  * Radius between faces, where both are vertical or near vertical
    * Detect by looking for a surface with edges that are arcs with a common radius, with arcs in XY plane.
    * Action: Select bit with radius <= arc radius OR error.  G-code should be G02, G03 rather than G01.

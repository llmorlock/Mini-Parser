# Mini-Parser
COP 4020: Principles of Programming Languages (Fall 2022)

Parser intakes statements of the form:
    prog        ->  begin stmt_list end

    stmt_list   ->  stmt ; stmt_list
                 |  stmt
                 |	stmt;

    stmt        ->  var = expr

    var         ->  A | B | C

    expr        ->  var + expr
                 |  var - expr
                 |  var
  
  It will tell you if the syntax for the statements from the selected file is correct or incorrect.
  If correct, it will output the statements in postfix format.

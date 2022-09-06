#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <utility>
#include <functional>
#include <iostream>
#include <exception>
#include <algorithm>
#include <sstream>

#include "slpy-ast.hh"

std::string re_escape(std::string s) {
    std::stringstream re_s;
    for (char c: s) {
        if (c == '\n') {
            re_s << "\\n";
        } else if (c == '\t') {
            re_s << "\\t";
        } else if (c == '\\') {
            re_s << "\\\\";
        } else if (c == '"') {
            re_s << "\\\"";
        } else {
            re_s << c;
        }
    }
    return re_s.str();
}

Prgm::Prgm(Blck_ptr mn) :
    main {mn}
{ }

void Stmt::output(std::ostream& os) const {
    output(os,"");
}

void Asgn::output(std::ostream& os, std::string indent) const {
    os << indent;
    os << name << " = ";
    expn->output(os);
    os << std::endl;
}

void Asgn::exec(Ctxt& ctxt) const {
    ctxt[name] = expn->eval(ctxt);
}

void Pass::output(std::ostream& os, std::string indent) const {
    os << indent << "pass" << std::endl;
}

void Pass::exec(Ctxt& ctxt) const {
    // does nothing!
}
  
void Prnt::output(std::ostream& os, std::string indent) const {
    os << indent;
    os << "print";
    os << "(";
    expn->output(os);
    os << ")";
    os << std::endl;
}

void Prnt::exec(Ctxt& ctxt) const {
    std::cout << expn->eval(ctxt) << std::endl;
}

void Blck::output(std::ostream& os, std::string indent) const {
    for (Stmt_ptr s : stmts) {
        s->output(os,indent);
    }
}

void Blck::output(std::ostream& os) const {
    for (Stmt_ptr s : stmts) {
        s->output(os);
    }
}

void Blck::exec(Ctxt& ctxt) const {
    for (Stmt_ptr s : stmts) {
        s->exec(ctxt);
    }
}

void Plus::output(std::ostream& os) const {
    os << "(";
    left->output(os);
    os << " + ";
    rght->output(os);
    os << ")";
}

int Plus::eval(const Ctxt& ctxt) const {
    int lv = left->eval(ctxt);
    int rv = rght->eval(ctxt);
    return (lv + rv);
}

void Mnus::output(std::ostream& os) const {
    os << "(";
    left->output(os);
    os << " - ";
    rght->output(os);
    os << ")";
}

int Mnus::eval(const Ctxt& ctxt) const {
    int lv = left->eval(ctxt);
    int rv = rght->eval(ctxt);
    return (lv - rv);
}

void Tmes::output(std::ostream& os) const {
    os << "(";
    left->output(os);
    os << " * ";
    rght->output(os);
    os << ")";
}

int Tmes::eval(const Ctxt& ctxt) const {
    int lv = left->eval(ctxt);
    int rv = rght->eval(ctxt);
    return (lv * rv);
}

void IDiv::output(std::ostream& os) const {
    os << "(";
    left->output(os);
    os << " // ";
    rght->output(os);
    os << ")";
}

int IDiv::eval(const Ctxt& ctxt) const {
    int lv = left->eval(ctxt);
    int rv = rght->eval(ctxt);
    return (lv / rv);
}

void Nmbr::output(std::ostream& os) const {
    os << std::to_string(valu);
}

int Nmbr::eval(const Ctxt& ctxt) const {
    return valu;
}

void Lkup::output(std::ostream& os) const {
    os << name;
}

int Lkup::eval(const Ctxt& ctxt) const {
    return ctxt.at(name);
}

void Inpt::output(std::ostream& os) const {
    os << "input(\"" << re_escape(prpt) << "\")";
}

int Inpt::eval(const Ctxt& ctxt) const {
    int vl;
    std::cout << prpt;
    std::cin >> vl;
    return vl;
}

void Prgm::output(std::ostream& os) const {
    main->output(os);
}

void Prgm::run(void) const {
    Ctxt main_ctxt { };
    main->exec(main_ctxt);
}


// Methods for dumping

void Prgm::dump(std::ostream& os) const {
    dump(os, "");
}

void Prgm::dump(std::ostream& os, std::string indent) const {
    os << "Prgm" << std::endl;
    main->dump(os, "    ");
}

void Blck::dump(std::ostream& os, std::string indent) const {
    os << indent << "Blck" << std::endl;
    indent += "    ";
    for (Stmt_ptr it : stmts) {
        it->dump(os, indent);
    }
}

// Stmt

void Asgn::dump(std::ostream& os, std::string indent) const {
    os << indent << "Asgn" << std::endl;
    indent += "    ";
    os << indent << name << std::endl;
    expn->dump(os, indent);
}


void Prnt::dump(std::ostream& os, std::string indent) const {
    os << indent << "Prnt" << std :: endl;
    indent += "    ";
    expn->dump(os,indent);
    os << std::endl;
}


void Pass::dump(std::ostream& os, std::string indent) const {
    os << indent << "Pass" << std::endl;
}

// Expn

void Plus::dump(std::ostream& os, std::string indent) const {
    os << indent << "Plus" << std:: endl;
    indent += "    ";
    left->dump(os, indent);
    rght->dump(os, indent);
}

void Mnus::dump(std::ostream& os, std::string indent) const {
    os << indent << "Mnus" << std:: endl;
    indent += "    ";
    left->dump(os, indent);
    rght->dump(os, indent);
}

void Tmes::dump(std::ostream& os, std::string indent) const {
    os << indent << "Tmes" << std:: endl;
    indent += "    ";
    left->dump(os, indent);
    rght->dump(os, indent);
}

void IDiv::dump(std::ostream& os, std::string indent) const {
    os << indent << "IDiv" << std:: endl;
    indent += "    ";
    left->dump(os, indent);
    rght->dump(os, indent);
}

void Nmbr::dump(std::ostream& os, std::string indent) const {
    os << indent << "Nmbr" << std:: endl;
    os << indent + "    " << valu << std::endl;
}

void Lkup::dump(std::ostream& os, std::string indent) const {
    os << indent << "Lkup" << std::endl;
    os << indent + "    "<< name << std::endl; 
}

void Inpt::dump(std::ostream& os, std::string indent) const {
    os << indent << "Inpt" << std:: endl;
    os << indent + "    " << "\"" << prpt << "\"" << std::endl;
}
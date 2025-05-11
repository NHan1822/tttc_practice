#include "clang/AST/AST.h"
#include "clang/AST/Attr.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;

namespace {

class MarkUnusedVarsVisitor : public RecursiveASTVisitor<MarkUnusedVarsVisitor> {
public:
    explicit MarkUnusedVarsVisitor(ASTContext &Context, Rewriter &R)
        : Context(Context), TheRewriter(R) {}

    bool VisitVarDecl(VarDecl *VD) {
        if (VD->isLocalVarDecl() && !VD->isUsed() && !VD->hasAttr<UnusedAttr>()) {
            handleUnusedDecl(VD);
        }
        return true;
    }

    bool VisitParmVarDecl(ParmVarDecl *PVD) {
        if (!PVD->isUsed() && !PVD->hasAttr<UnusedAttr>()) {
            handleUnusedDecl(PVD);
        }
        return true;
    }

private:
    void handleUnusedDecl(Decl *D) {
        SourceLocation Loc = D->getBeginLoc();
        if (Loc.isInvalid() || Loc.isMacroID()) {
            return;
        }
        TheRewriter.InsertText(Loc, "[[maybe_unused]] ", true, true);
        D->addAttr(UnusedAttr::CreateImplicit(Context, D->getLocation()));
    }

    ASTContext &Context;
    Rewriter &TheRewriter;
};

class MarkUnusedVarsConsumer : public ASTConsumer {
public:
    MarkUnusedVarsConsumer(ASTContext &Context, Rewriter &R)
        : Visitor(Context, R) {}

    void HandleTranslationUnit(ASTContext &Context) override {
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }
private:
    MarkUnusedVarsVisitor Visitor;
};

class MarkUnusedVarsAction : public PluginASTAction {
    public:
        void EndSourceFileAction() override {
            TheRewriter.getEditBuffer(TheRewriter.getSourceMgr().getMainFileID())
                .write(llvm::outs());
            getCompilerInstance().getASTContext().getTranslationUnitDecl()->dump();
        }

        bool ParseArgs(const CompilerInstance &CI,
                       const std::vector<std::string> &args) override {
            return true;
        }
    
        std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, 
                                                       llvm::StringRef InFile) override {
            TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
            return std::make_unique<MarkUnusedVarsConsumer>(CI.getASTContext(), TheRewriter);
        }
    private:
        Rewriter TheRewriter;
    };

} // namespace

static FrontendPluginRegistry::Add<MarkUnusedVarsAction>
    X("mark-unused", "Add [[maybe_unused]] to unused vars and params");

#include "TabsInsteadOfSpacesCheck.h>
#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"

namespace clang {
namespace tidy {
namespace tabsinsteadofspaces {

class TabsInsteadOfSpacesModule : public ClangTidyModule {
public:
    void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
        CheckFactories.registerCheck<TabsInsteadOfSpacesCheck>("tabsinsteadofspaces-check");
    }
};

} // namespace tabsinsteadofspaces
} // namespace tidy
} // namespace clang

// Register the TabsInsteadOfSpacesModule using this statically initialized variable.
static clang::tidy::ClangTidyModuleRegistry::Add<clang::tidy::tabsinsteadofspaces::TabsInsteadOfSpacesModule>
    X("tabsinsteadofspaces-module", "checks for code using spaces instead of tabs.");

// This anchor is used to force the linker to link in the generated object file (had some help with this one)
volatile int TabsInsteadOfSpacesModuleAnchorSource = 0;

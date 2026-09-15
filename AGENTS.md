# WantedFPS Codex Instructions


## Project Overview


This repository is an Unreal Engine 5 C++ game project named `WantedFPS`.


The Unreal project file is:


`WantedFPS.uproject`


The primary gameplay C++ module is:


`Source/WantedFPS/`


The project contains both C++ gameplay code and Unreal Engine binary assets such as Blueprints, maps, animations, materials, meshes, UI assets, input assets, and external actors.


Prefer C++ implementations unless the user explicitly requests otherwise.


---


# Source of Truth


Always treat the current files on disk as the source of truth.


The user may edit files manually in Visual Studio or Unreal Editor between Codex tasks.


Before modifying a file that may have been changed previously:


1. Re-read the current file from disk.

2. Check the current Git diff when relevant.

3. Do not rely only on an earlier version remembered from the conversation.


If the user says they changed something manually, inspect the current files again before continuing.


---


# Primary Writable Area


Codex may normally inspect and modify files under:


`Source/WantedFPS/`


This is the main C++ gameplay code directory.


Relevant areas currently include:


* `Source/WantedFPS/CurveBullet.cpp`

* `Source/WantedFPS/CurveBullet.h`

* `Source/WantedFPS/WantedFPSCharacter.*`

* `Source/WantedFPS/WantedFPSPlayerController.*`

* `Source/WantedFPS/WantedFPSGameMode.*`

* `Source/WantedFPS/WantedFPSCameraManager.*`


Shooter-specific code is located under:


`Source/WantedFPS/Variant_Shooter/`


including:


* Characters

* AI

* Weapons

* Projectiles

* UI

* Player controller

* Game mode


Horror-specific code is located under:


`Source/WantedFPS/Variant_Horror/`


Only modify files relevant to the requested task.


Do not refactor unrelated systems.


---


# Build Configuration


The following files may be modified when necessary for a requested C++ implementation:


* `Source/WantedFPS/WantedFPS.Build.cs`

* `Source/WantedFPS.Target.cs`

* `Source/WantedFPSEditor.Target.cs`


Before adding or removing a module dependency in `WantedFPS.Build.cs`, determine why it is required.


Do not add dependencies merely to fix compiler errors without understanding the cause.


Explain any Build.cs or Target.cs changes in the final report.


---


# Unreal Config Files


Files under:


`Config/`


must be treated carefully.


Codex may inspect them.


Only modify Config files when the requested feature genuinely requires a configuration change.


Relevant tracked files currently include:


* `Config/DefaultEditor.ini`

* `Config/DefaultEditorPerProjectUserSettings.ini`

* `Config/DefaultEngine.ini`

* `Config/DefaultGame.ini`

* `Config/DefaultInput.ini`


Do not make unrelated configuration changes.


Always report Config changes explicitly.


---


# Unreal Content


Treat the entire:


`Content/`


directory as read-only unless the user explicitly authorizes a specific asset change.


The Content directory contains Unreal binary assets such as:


* `.uasset`

* `.umap`

* Blueprints

* Animation Blueprints

* Animations

* Control Rigs

* Materials

* Meshes

* Textures

* Niagara systems

* UI widgets

* Input Actions

* Input Mapping Contexts

* Data Tables

* State Trees

* EQS assets


Do not attempt to directly edit `.uasset` or `.umap` files as ordinary files.


Do not create, delete, rename, move, replace, or overwrite Unreal assets unless explicitly requested.


If a C++ implementation requires a Blueprint or Unreal Editor change:


1. Implement the C++ portion if appropriate.

2. Do not modify the binary asset automatically.

3. Clearly explain what the user must configure in Unreal Editor.


For example, explain required:


* Blueprint parent class changes

* Component assignments

* Socket names

* Input mappings

* exposed property values

* Data Table references

* Projectile classes

* Animation settings


instead of attempting unsafe binary edits.


---


# Maps and External Actors


Never directly modify files under:


* `Content/__ExternalActors__/`

* `Content/__ExternalObjects__/`


These files are generated and managed by Unreal Engine.


Do not delete, rename, move, or manually rewrite them.


Do not attempt to clean these directories automatically.


Map changes should normally be performed by the user through Unreal Editor unless explicitly requested otherwise.


---


# Project File


Treat:


`WantedFPS.uproject`


as protected.


Do not modify it unless the requested task requires a project-level change such as a plugin or module configuration change.


Never change the Unreal Engine version without explicit user permission.


Never enable, disable, install, or remove Unreal Engine plugins without explicit user permission.


---


# Generated and Local Files


Do not modify or intentionally generate files in Unreal or Visual Studio generated directories unless required by a user-requested build.


Examples include:


* `Binaries/`

* `Intermediate/`

* `DerivedDataCache/`

* `Saved/`

* `.vs/`


Do not add these directories to Git.


Do not alter `.gitignore` simply to make generated files trackable.


---


# C++ Development Rules


Follow Unreal Engine C++ conventions.


Prefer existing project architecture over introducing new architectural patterns.


Before adding a new class, first determine whether the requested behavior belongs in an existing class.


Keep changes focused and minimal.


Do not perform broad cleanup or refactoring unless explicitly requested.


Do not rename public classes, Unreal reflected properties, reflected functions, assets, sockets, or Blueprint-facing identifiers without considering Unreal serialization and Blueprint references.


Be especially careful when changing:


* `UCLASS`

* `USTRUCT`

* `UENUM`

* `UPROPERTY`

* `UFUNCTION`


Do not remove reflection macros merely to simplify code.


Preserve Blueprint compatibility where reasonable.


---


# Gameplay Parameters


Gameplay tuning values that are likely to change during playtesting should normally be exposed appropriately rather than buried as unexplained magic numbers.


When appropriate, prefer Unreal properties such as:


`UPROPERTY(EditAnywhere, BlueprintReadWrite, ...)`


or a more restrictive suitable variant.


Do not expose every internal variable unnecessarily.


Use the narrowest appropriate visibility.


Examples of likely tuning parameters include:


* projectile speed

* projectile lifetime

* curve strength

* turn rate

* maximum travel distance

* damage

* cooldown

* weapon spread

* camera offsets


---


# CurveBullet


`CurveBullet.cpp` and `CurveBullet.h` contain project-specific curved projectile behavior.


When modifying curved projectile behavior:


1. Inspect the current implementation first.

2. Preserve unrelated projectile behavior.

3. Prefer configurable parameters for values expected to be adjusted through playtesting.

4. Do not assume previous experimental values are final.

5. Explain the geometric or gameplay meaning of important parameters when changing them.


---


# Shooter Variant


Shooter-related gameplay code exists under:


`Source/WantedFPS/Variant_Shooter/`


Relevant systems include:


* AI

* Shooter character

* Shooter controller

* Weapons

* Weapon holder

* Pickups

* Projectiles

* Shooter UI


Before changing shooting behavior, inspect the relevant weapon, projectile, character, and holder classes as needed.


Do not assume one class owns the entire shooting pipeline without checking the implementation.


---


# Blueprint Coordination


Many corresponding Unreal assets exist under:


`Content/Variant_Shooter/`


and other Content directories.


C++ and Blueprint behavior may depend on each other.


If the requested implementation depends on a Blueprint setting that Codex cannot safely modify, state exactly what must be changed in Unreal Editor.


Do not pretend the feature is fully complete if manual Blueprint configuration is still required.


---


# Investigation Before Implementation


For non-trivial changes, use this workflow:


1. Inspect the relevant files.

2. Identify which classes currently implement the behavior.

3. Explain or internally establish the smallest reasonable implementation approach.

4. Modify only relevant files.

5. Build or compile when reasonably possible.

6. Inspect errors if the build fails.

7. Fix errors caused by the change when possible.

8. Review the Git diff.

9. Report the result.


For large or potentially architectural changes, do not immediately rewrite the project.


First describe the proposed approach and affected files.


---


# Builds and Validation


A successful code edit is not the same as a successful Unreal Engine feature.


When possible, validate changes by compiling the appropriate Unreal target.


Do not claim:


* "the project builds"

* "the feature works"

* "the issue is fixed"


unless that claim was actually verified.


Distinguish between:


* code written

* compilation verified

* Unreal Editor startup verified

* gameplay behavior verified


Gameplay behavior normally requires user testing in Unreal Editor.


If the local Unreal Engine installation path or build command is unknown, do not invent one.


Ask for or discover the correct environment information before relying on a guessed engine path.


---


# Build Failure Policy


If compilation fails:


1. Read the actual compiler output.

2. Identify the first meaningful errors rather than blindly changing many files.

3. Determine whether the errors were introduced by the current change.

4. Fix relevant errors.

5. Rebuild when appropriate.


Do not hide remaining errors.


Report unresolved errors clearly.


---


# Git Safety


Git is used as the safety boundary for this project.


Before significant edits, inspect:


`git status`


When useful, inspect:


`git diff`


After implementation, review the final diff before reporting completion.


Never execute destructive Git commands without explicit user permission.


Do not run commands such as:


* `git reset --hard`

* `git clean -fd`

* `git clean -fdx`

* `git checkout -- .`

* `git restore .`

* `git push --force`

* `git push --force-with-lease`


Do not discard the user's uncommitted changes.


Do not overwrite manual changes simply because they differ from a previous Codex-generated version.


---


# Commits and Remote Operations


Do not create Git commits unless the user explicitly asks for a commit.


Do not push to GitHub unless explicitly requested.


Do not modify remote branches unless explicitly requested.


If the user asks for a commit message, provide an appropriate commit message based on the actual changes.


---


# Existing User Changes


Assume uncommitted changes may belong to the user.


Never revert or overwrite them without understanding them.


If the working tree already contains changes unrelated to the current task:


* leave them intact

* avoid including unrelated modifications

* work around them where possible


If a conflict exists between the requested implementation and existing user modifications, explain the conflict.


---


# Scope Control


For each task, make the smallest set of changes necessary.


Do not:


* redesign unrelated gameplay systems

* rename unrelated files

* reformat the entire codebase

* change code style globally

* update unrelated dependencies

* modify unrelated Config values

* modify unrelated Unreal assets


A focused diff is preferred over a large cleanup.


---


# Codex Usage Efficiency


Do not recursively inspect the entire `Content/` directory for ordinary C++ tasks.


Most Content files are binary Unreal assets and provide little value to text-based code analysis.


Start investigation from:


`Source/WantedFPS/`


and narrow the search based on the requested feature.


Inspect Config or Content paths only when they are relevant to the task.


Avoid repeatedly re-reading large unrelated parts of the repository.


---


# Communication


When a task is complete, provide a concise summary containing:


1. What was changed.

2. Which files were changed.

3. Why those files were changed.

4. Whether compilation/build was actually run.

5. Whether compilation succeeded.

6. Any Unreal Editor or Blueprint steps the user still needs to perform.

7. Any known limitations or remaining issues.


Do not report assumptions as confirmed facts.


If something requires gameplay testing, explicitly say that gameplay testing is still required.

